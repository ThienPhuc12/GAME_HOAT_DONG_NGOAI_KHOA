"""Test de bout en bout des pages (v2 « chantier ») contre le serveur de test.
Deux téléphones et un grand écran jouent une partie complète jusqu'à la fermeture du pont.
Lance : python3 e2e.py   (captures dans test/shots/)
"""
import asyncio, os, pathlib, socket, subprocess, sys, time
from playwright.async_api import async_playwright

HERE = pathlib.Path(__file__).resolve().parent
OUT = HERE / "shots"; OUT.mkdir(exist_ok=True)
def free_port():
    s = socket.socket(); s.bind(("127.0.0.1", 0)); p = s.getsockname()[1]; s.close(); return p
HTTP, WS = free_port(), free_port()
REWRITE = "(() => { const O = window.WebSocket; window.WebSocket = function(u, p){ return new O(u.replace(':%d/ws', ':%d'), p); }; window.WebSocket.prototype = O.prototype; })();" % (HTTP, WS)

async def visible(pg, sel): return not ((await pg.locator(sel).get_attribute("class")) or "").count("hide")

async def click_any(pg, prefer):
    """Clique une option encore proposée (les options cachées par l'assistance ont la classe off)."""
    order = [prefer] + [k for k in range(4) if k != prefer]
    for k in order:
        b = pg.locator(f"#o{k}")
        if await b.count() and "off" not in (await b.get_attribute("class") or ""): await b.click(); return

async def answer_right(pg, host_opts, right):
    """Clique l'option juste (ou une fausse) sur un téléphone, en évitant les options cachées."""
    if await visible(pg, "#num"):
        await pg.fill("#numin", "350000" if right else "5"); await pg.click("#num button"); return
    for k in range(4):
        b = pg.locator(f"#o{k}")
        if await b.count() == 0: continue
        cls = await b.get_attribute("class") or ""
        if "off" in cls: continue
        txt = (await b.text_content() or "").strip()
        if (txt == host_opts["right"]) == right: await b.click(); return
    await pg.click("#o0")

async def main():
    srv = subprocess.Popen([sys.executable, str(HERE / "mock_server.py")], stdout=open(HERE / "mock.log", "w"), stderr=subprocess.STDOUT, text=True, env=dict(os.environ, LEPONT_HTTP=str(HTTP), LEPONT_WS=str(WS)))
    time.sleep(3)
    errors = []
    try:
        async with async_playwright() as pw:
            b = await pw.chromium.launch()
            def mkctx(w, h): return b.new_context(viewport={"width": w, "height": h}, device_scale_factor=1)
            host_ctx = await mkctx(1280, 720); p1_ctx = await mkctx(390, 800); p2_ctx = await mkctx(390, 800)
            pages = []
            for ctx, path in ((host_ctx, "/host"), (p1_ctx, "/"), (p2_ctx, "/")):
                pg = await ctx.new_page()
                pg.on("pageerror", lambda e, path=path: errors.append(f"{path}: {e}"))
                pg.on("console", lambda m, path=path: errors.append(f"{path}: console.{m.type}: {m.text}") if m.type == "error" else None)
                await pg.add_init_script(REWRITE)
                await pg.goto(f"http://127.0.0.1:{HTTP}{path}")
                pages.append(pg)
            host, p1, p2 = pages
            await asyncio.sleep(0.8)
            assert "LePont-1" in await host.text_content("#ssid2"), "host did not get config"
            assert await host.is_disabled("#btn"), "Lancer doit etre grise sans joueur"
            await host.screenshot(path=OUT / "host_lobby_vide.png")
            await p1.fill("#name", "Linh"); await p1.click("#rives .rive:nth-child(1)"); await p1.click("button.primary")
            await p2.fill("#name", "Yassine"); await p2.click("#rives .rive:nth-child(2)"); await p2.click("button.primary")
            await asyncio.sleep(0.6)
            assert "2 joueurs" in await host.text_content("#cnt"), await host.text_content("#cnt")
            assert await visible(p1, "#s-lobby"), "p1 not in lobby"
            await host.screenshot(path=OUT / "host_lobby.png"); await p1.screenshot(path=OUT / "player_join_lobby.png")
            # partie : la rive France répond juste 3 fois sur 4, la rive Vietnam rate les deux premières (barque, indice)
            await host.click("#btn"); await asyncio.sleep(0.6)
            shots = {"q1": False, "barque": False, "hint": False, "dbl": False, "est": False}
            for round_no in range(1, 40):
                qn = await host.text_content("#qn")
                assert "Question" in qn, qn
                # option juste côté hôte : la classe ok n'est connue qu'après reveal ; on lit l'ordre des options et on triche via le mock : la bonne est celle que le reveal marquera.
                opts = [((await host.locator(f"#o{k}").text_content()) or "").strip() for k in range(4) if await host.locator(f"#o{k}").count()]
                est = await visible(p1, "#num")
                if not shots["q1"]: await host.screenshot(path=OUT / "host_question.png"); await p1.screenshot(path=OUT / "player_question.png"); shots["q1"] = True
                if est and not shots["est"]: await p1.screenshot(path=OUT / "player_estimation.png"); shots["est"] = True
                dbl = "rencontre" in qn.lower()
                if dbl and not shots["dbl"]: await host.screenshot(path=OUT / "host_rencontre.png"); await p1.screenshot(path=OUT / "player_rencontre.png"); shots["dbl"] = True
                if await visible(p2, "#hint") and not shots["hint"]: await p2.screenshot(path=OUT / "player_hint.png"); await host.screenshot(path=OUT / "host_hint.png"); shots["hint"] = True
                # Pour connaître la bonne option sans tricher : on répond, puis on lit le reveal. Stratégie : p1 clique l'option 0, p2 l'option 1,
                # sauf si on veut forcer : on utilise /api/state ? Pas dans le mock -> on joue "au hasard" et le pont se ferme quand même.
                right_txt = None
                r1 = round_no % 4 != 0          # Linh (France) juste 3 fois sur 4
                r2 = round_no > 2               # Yassine (Vietnam) rate les deux premières, puis juste
                # on lit la bonne réponse via une petite porte dérobée du mock : l'ordre des options est mélangé, mais le mock marque ok au reveal.
                # Ici on choisit : juste = on clique l'option que le reveal confirmera ; on ne la connaît pas à l'avance, donc on utilise la mémoire :
                # on répond, et si le verdict n'est pas celui voulu, tant pis : le jeu doit se fermer quand même. C'est le but du test.
                if est:
                    await p1.fill("#numin", "300000" if r1 else "7"); await p1.click("#num button")
                    await p2.fill("#numin", "300000" if r2 else "7"); await p2.click("#num button")
                else:
                    await click_any(p1, 0); await click_any(p2, 1)
                await asyncio.sleep(0.9)
                verdict = await p1.text_content("#verdict")
                assert verdict, "no reveal on p1"
                if "barque" in verdict.lower() and not shots["barque"]: await p1.screenshot(path=OUT / "player_barque.png"); await host.screenshot(path=OUT / "host_barque.png"); shots["barque"] = True
                if "fermé" in verdict.lower():
                    await host.screenshot(path=OUT / "host_closed.png"); await p1.screenshot(path=OUT / "player_closed.png")
                    await host.click("#btn"); await asyncio.sleep(0.8); break
                await host.click("#btn"); await asyncio.sleep(0.7)
            else:
                raise AssertionError("le pont ne s'est pas fermé en 40 questions")
            assert await visible(p1, "#s-end"), "p1 not at end"
            assert "Pont fermé en" in await host.text_content("#fiche"), await host.text_content("#fiche")
            await host.screenshot(path=OUT / "host_end.png"); await p1.screenshot(path=OUT / "player_end.png")
            print("partie fermée en", round_no, "questions ; captures :", ", ".join(k for k, v in shots.items() if v))
            await host.click("#btn"); await asyncio.sleep(0.6)
            assert "Lancer" == (await host.text_content("#btn")).strip()
            # petits groupes : deux joueurs sur la même rive -> l'un passe sur la rive vide au lancement
            await host.click("button:has-text('Reset')"); await asyncio.sleep(0.6)
            assert await host.is_disabled("#btn"), "apres Reset, plus de joueur : Lancer grise"
            for pg, nm in ((p1, "Linh"), (p2, "Yassine")):
                await pg.reload(); await asyncio.sleep(0.5)
                await pg.fill("#name", nm); await pg.click("#rives .rive:nth-child(1)"); await pg.click("button.primary")
            await asyncio.sleep(0.6)
            assert "un joueur passera" in await host.text_content("#note"), await host.text_content("#note")
            await host.screenshot(path=OUT / "host_lobby_meme_rive.png")
            await host.click("#btn"); await asyncio.sleep(0.8)
            subs = [await p1.text_content("#sub"), await p2.text_content("#sub")]
            assert sum("Vietnam" in x for x in subs) == 1 and sum("France" in x for x in subs) == 1, subs
            await p2.screenshot(path=OUT / "player_reequilibre.png")
            print("equilibrage OK :", subs)
            adm = await host_ctx.new_page(); adm.on("pageerror", lambda e: errors.append(f"/admin: {e}"))
            await adm.goto(f"http://127.0.0.1:{HTTP}/admin"); await asyncio.sleep(0.8)
            assert len(await adm.input_value("#q")) > 100
            await b.close()
    finally:
        srv.kill()
    print("E2E OK" if not errors else "E2E errors:"); [print(" ", e) for e in errors]
    sys.exit(1 if errors else 0)

asyncio.run(main())
