#pragma once
// =============================================================================
//  LE PONT · CẦU — base de questions (JSON embarqué dans le programme)
//  VI: bộ câu hỏi (JSON nhúng trong chương trình)
//  Fichier GÉNÉRÉ par gen_questions_h.py depuis 05_Questions/PONT_questions_FR_VI.json
//  VI: Tệp được TẠO TỰ ĐỘNG bởi gen_questions_h.py từ 05_Questions/PONT_questions_FR_VI.json
//
//  Format d'une question · Định dạng một câu hỏi :
//   {"cat":"Catégorie","fr":"Question en français","vi":"Câu hỏi tiếng Việt",
//    "o":["A","B","C","D"],"r":0,"lv":2,"pfr":"Révélation FR","pvi":"Tiết lộ VI"}
//   r = index de la bonne réponse (0 à 3) ; lv = niveau 1 facile, 2 moyen, 3 difficile
//   VI: r = chỉ số của đáp án đúng (0 đến 3); lv = cấp độ 1 dễ, 2 trung bình, 3 khó
//   Estimation : pas de "o", mais "num" = valeur cible (les deux rives tapent un nombre).
//   VI: Câu ước lượng: không có "o", mà có "num" = giá trị đích (hai bờ nhập một con số).
//   "hfr" (facultatif) = indice du chef de chantier ; sinon la 1re phrase de "pfr" sert d'indice.
//   VI: "hfr" (không bắt buộc) = gợi ý của đốc công; nếu không có, câu đầu tiên của "pfr" được dùng.
//  Tu peux aussi charger des questions sans reflasher : http://192.168.4.1/admin
//  VI: Có thể tải câu hỏi mà không cần nạp lại chương trình: http://192.168.4.1/admin
// =============================================================================
const char QUESTIONS_JSON[] PROGMEM = R"json(
[
 {
  "cat": "Mots voyageurs",
  "fr": "Le mot vietnamien « bê tông » vient de quel mot français ?",
  "vi": "Từ « bê tông » trong tiếng Việt bắt nguồn từ từ tiếng Pháp nào?",
  "o": [
   "béton",
   "bateau",
   "bouton",
   "bidon"
  ],
  "r": 0,
  "pfr": "Les ingénieurs vietnamiens construisent en « bê tông cốt thép » (béton armé) : le mot est arrivé avec les premiers ponts en acier et béton, il y a plus d'un siècle. « Xi măng » (ciment) a fait le même voyage.",
  "pvi": "Kỹ sư Việt Nam xây bằng « bê tông cốt thép »: chính từ này đã đến cùng những cây cầu thép và bê tông đầu tiên, hơn một thế kỷ trước. « Xi măng » (ciment) cũng đi cùng con đường ấy.",
  "lv": 1
 },
 {
  "cat": "Mots voyageurs",
  "fr": "Parmi ces mots vietnamiens, lequel ne vient PAS du français ?",
  "vi": "Từ nào sau đây KHÔNG bắt nguồn từ tiếng Pháp?",
  "o": [
   "ga",
   "xà phòng",
   "cà phê",
   "nón lá"
  ],
  "r": 3,
  "pfr": "Ga (gare), xà phòng (savon) et cà phê (café) sont entrés avec le chemin de fer, le savon de Marseille et le café. Le nón lá, le chapeau conique, est bien plus ancien.",
  "pvi": "Ga (gare), xà phòng (savon), cà phê (café) đến cùng đường sắt, xà phòng Marseille và cà phê. Nón lá thì có từ lâu đời hơn nhiều.",
  "lv": 1
 },
 {
  "cat": "Mots voyageurs",
  "fr": "Le bánh mì descend de quel pain français ?",
  "vi": "Bánh mì có nguồn gốc từ loại bánh nào của Pháp?",
  "o": [
   "la baguette / bánh baguette",
   "le pain de campagne",
   "la brioche",
   "le pain de mie / bánh mì gối"
  ],
  "r": 0,
  "pfr": "La baguette arrive avec les boulangers français au XIXe siècle ; les Vietnamiens l'ont garnie de pâté, de légumes marinés et de coriandre. En 2011, « banh mi » entre dans l'Oxford English Dictionary.",
  "pvi": "Bánh baguette đến cùng các thợ làm bánh Pháp thế kỷ XIX; người Việt kẹp pa-tê, đồ chua và rau mùi. Năm 2011, « banh mi » được đưa vào từ điển Oxford.",
  "lv": 1
 },
 {
  "cat": "Mots voyageurs",
  "fr": "Lequel de ces mots vietnamiens figure dans le dictionnaire Larousse ?",
  "vi": "Từ tiếng Việt nào sau đây có trong từ điển Larousse của Pháp?",
  "o": [
   "nước mắm",
   "xe máy",
   "bún",
   "cầu"
  ],
  "r": 0,
  "pfr": "Le voyage des mots se fait dans les deux sens : nuoc-mâm, nem et pho sont aujourd'hui dans les dictionnaires français.",
  "pvi": "Từ ngữ đi cả hai chiều: nuoc-mâm, nem và pho nay đều có trong từ điển tiếng Pháp.",
  "lv": 2
 },
 {
  "cat": "Mots voyageurs",
  "fr": "Pourquoi le cà phê sữa đá se prépare-t-il au lait concentré sucré ?",
  "vi": "Vì sao cà phê sữa đá được pha bằng sữa đặc có đường?",
  "o": [
   "le lait frais n'existait pas en Indochine / vì thiếu sữa tươi",
   "c'est plus sucré / vì ngọt hơn",
   "le lait concentré venait de Chine / sữa đặc đến từ Trung Quốc",
   "par tradition bouddhiste / theo truyền thống Phật giáo"
  ],
  "r": 0,
  "pfr": "Les Français plantent les premiers caféiers vers 1857. Sans chaîne du froid, pas de lait frais : on prend le lait concentré en boîte. Le Vietnam est aujourd'hui le 2e producteur mondial de café (robusta).",
  "pvi": "Người Pháp trồng những cây cà phê đầu tiên khoảng năm 1857. Không có dây chuyền lạnh, không có sữa tươi, nên người ta dùng sữa đặc đóng hộp. Việt Nam nay là nước sản xuất cà phê lớn thứ hai thế giới (robusta).",
  "lv": 2
 },
 {
  "cat": "Mots voyageurs",
  "fr": "L'alphabet vietnamien (quốc ngữ) a été codifié au XVIIe siècle par un jésuite né à Avignon. Qui ?",
  "vi": "Chữ Quốc ngữ được hệ thống hóa vào thế kỷ XVII bởi một tu sĩ Dòng Tên sinh ở Avignon. Ông là ai?",
  "o": [
   "Alexandre de Rhodes",
   "Alexandre Yersin",
   "Francis Garnier",
   "Pigneau de Béhaine"
  ],
  "r": 0,
  "pfr": "Créé par des missionnaires portugais puis codifié par de Rhodes dans son dictionnaire de 1651, le quốc ngữ fait du Vietnam le seul pays d'Asie continentale à écrire en alphabet latin. Il n'est devenu l'écriture officielle qu'au XXe siècle.",
  "pvi": "Do các giáo sĩ Bồ Đào Nha khởi tạo và được de Rhodes hệ thống hóa trong từ điển năm 1651, chữ Quốc ngữ khiến Việt Nam là nước duy nhất ở lục địa châu Á dùng chữ Latinh. Chữ này chỉ trở thành chính thức vào thế kỷ XX.",
  "lv": 3
 },
 {
  "cat": "Portraits croisés",
  "fr": "Alexandre Yersin, médecin franco-suisse, a découvert le bacille de la peste en 1894. Où est-il enterré ?",
  "vi": "Alexandre Yersin, bác sĩ Pháp–Thụy Sĩ, tìm ra trực khuẩn dịch hạch năm 1894. Ông được an táng ở đâu?",
  "o": [
   "Nha Trang (Suối Dầu)",
   "Paris",
   "Lausanne",
   "Đà Lạt"
  ],
  "r": 0,
  "pfr": "Installé à Nha Trang dès 1891 (Institut Pasteur), il explore le plateau du Lang Biang en 1893 : Đà Lạt naît de son rapport. Les Vietnamiens l'appellent « Ông Năm » ; sa tombe à Suối Dầu est un lieu de pèlerinage.",
  "pvi": "Định cư ở Nha Trang từ 1891 (Viện Pasteur), ông thám hiểm cao nguyên Lang Biang năm 1893: Đà Lạt ra đời từ báo cáo của ông. Người dân gọi ông là « Ông Năm »; mộ ông ở Suối Dầu là nơi hành hương.",
  "lv": 2
 },
 {
  "cat": "Portraits croisés",
  "fr": "Ngô Bảo Châu, médaille Fields 2010 : dans quelle université a-t-il soutenu sa thèse ?",
  "vi": "Ngô Bảo Châu, huy chương Fields 2010, bảo vệ luận án tiến sĩ ở đại học nào?",
  "o": [
   "Paris-Sud (Orsay)",
   "Sorbonne",
   "École polytechnique",
   "Université de Hanoï / Đại học Hà Nội"
  ],
  "r": 0,
  "pfr": "Premier Vietnamien médaillé Fields, formé à Orsay (thèse 1997 sous Gérard Laumon), naturalisé français en 2010. Il dirige aujourd'hui l'Institut d'études avancées en mathématiques de Hanoï (VIASM).",
  "pvi": "Người Việt đầu tiên nhận huy chương Fields, đào tạo tại Orsay (luận án 1997, thầy hướng dẫn Gérard Laumon), nhập quốc tịch Pháp năm 2010. Nay ông là giám đốc khoa học Viện Nghiên cứu cao cấp về Toán (VIASM) ở Hà Nội.",
  "lv": 3
 },
 {
  "cat": "Portraits croisés",
  "fr": "Marguerite Duras est née en 1914 près de Saigon. Quel roman, prix Goncourt 1984, raconte sa jeunesse en Indochine ?",
  "vi": "Marguerite Duras sinh năm 1914 gần Sài Gòn. Tiểu thuyết nào, giải Goncourt 1984, kể về tuổi trẻ của bà ở Đông Dương?",
  "o": [
   "L'Amant / Người tình",
   "Un barrage contre le Pacifique",
   "Indochine",
   "Le Vice-consul"
  ],
  "r": 0,
  "pfr": "« Un barrage contre le Pacifique » (1950) se passe aussi au Vietnam. « L'Amant » a été adapté au cinéma en 1992, tourné à Sa Đéc et à Saigon.",
  "pvi": "« Un barrage contre le Pacifique » (1950) cũng lấy bối cảnh Việt Nam. « Người tình » được chuyển thể thành phim năm 1992, quay ở Sa Đéc và Sài Gòn.",
  "lv": 2
 },
 {
  "cat": "Portraits croisés",
  "fr": "Trần Anh Hùng, cinéaste né à Mỹ Tho et formé à Paris : quel film tourné à Hồ Chí Minh-Ville a reçu le Lion d'or à Venise en 1995 ?",
  "vi": "Trần Anh Hùng, đạo diễn sinh ở Mỹ Tho và học ở Paris: phim nào quay tại TP. Hồ Chí Minh giành Sư tử Vàng Venice 1995?",
  "o": [
   "Cyclo / Xích lô",
   "L'Odeur de la papaye verte / Mùi đu đủ xanh",
   "À la verticale de l'été / Mùa hè chiều thẳng đứng",
   "Norwegian Wood / Rừng Na Uy"
  ],
  "r": 0,
  "pfr": "« L'Odeur de la papaye verte » (Caméra d'or à Cannes 1993) a été tourné… en studio à Boulogne-Billancourt. En 2023, il reçoit le prix de la mise en scène à Cannes pour « La Passion de Dodin Bouffant ».",
  "pvi": "« Mùi đu đủ xanh » (Caméra d'Or Cannes 1993) lại được quay… trong trường quay ở Boulogne-Billancourt, Pháp. Năm 2023 ông nhận giải Đạo diễn xuất sắc tại Cannes.",
  "lv": 2
 },
 {
  "cat": "Portraits croisés",
  "fr": "Albert Calmette a fondé l'Institut Pasteur de Saigon en 1891. Quel vaccin célèbre porte son nom ?",
  "vi": "Albert Calmette thành lập Viện Pasteur Sài Gòn năm 1891. Vắc-xin nổi tiếng nào mang tên ông?",
  "o": [
   "le BCG",
   "le vaccin contre la rage / vắc-xin dại",
   "le vaccin contre le tétanos / vắc-xin uốn ván",
   "le vaccin contre la fièvre jaune / vắc-xin sốt vàng"
  ],
  "r": 0,
  "pfr": "À Saigon, Calmette met au point le premier sérum antivenimeux contre les serpents. Le BCG (Bacille de Calmette et Guérin) naît 30 ans plus tard à Lille, et il est encore injecté aux bébés vietnamiens aujourd'hui.",
  "pvi": "Tại Sài Gòn, Calmette chế tạo huyết thanh kháng nọc rắn đầu tiên. BCG (Bacille de Calmette et Guérin) ra đời 30 năm sau ở Lille, và vẫn được tiêm cho trẻ em Việt Nam ngày nay.",
  "lv": 2
 },
 {
  "cat": "Portraits croisés",
  "fr": "Lê Phổ, Mai Trung Thứ et Vũ Cao Đàm, formés à l'École des Beaux-Arts de l'Indochine (fondée en 1925 par Victor Tardieu), ont fait carrière en France. Dans quelle technique excellaient-ils ?",
  "vi": "Lê Phổ, Mai Trung Thứ và Vũ Cao Đàm, tốt nghiệp Trường Mỹ thuật Đông Dương (Victor Tardieu lập năm 1925), đã lập nghiệp ở Pháp. Họ nổi tiếng với chất liệu nào?",
  "o": [
   "la peinture sur soie / tranh lụa",
   "la laque / sơn mài",
   "l'aquarelle / màu nước",
   "la gravure sur bois / khắc gỗ"
  ],
  "r": 0,
  "pfr": "Leurs toiles se vendent aujourd'hui des millions : un portrait de Lê Phổ a atteint 3,1 millions de dollars à Hong Kong en 2021.",
  "pvi": "Tranh của họ nay bán hàng triệu đô: một bức chân dung của Lê Phổ đạt 3,1 triệu USD tại Hồng Kông năm 2021.",
  "lv": 3
 },
 {
  "cat": "Pierres et ponts",
  "fr": "On attribue souvent à Gustave Eiffel le pont Long Biên (Hanoï), le pont Trường Tiền (Huế) et la Poste centrale de Saigon. Combien sont vraiment de lui ?",
  "vi": "Người ta thường gán cho Gustave Eiffel cầu Long Biên (Hà Nội), cầu Trường Tiền (Huế) và Bưu điện Sài Gòn. Thực tế bao nhiêu công trình là của ông?",
  "o": [
   "aucun / không có",
   "un / một",
   "deux / hai",
   "les trois / cả ba"
  ],
  "r": 0,
  "pfr": "Long Biên = Daydé & Pillé (1898-1903, 1 682 m). Trường Tiền = Schneider & Letellier (1897-1900). La Poste = l'architecte Marie-Alfred Foulhoux. L'entreprise Eiffel a seulement élargi Trường Tiền en 1937 ; Daydé a rejoint le groupe Eiffel en 1964, d'où la confusion.",
  "pvi": "Long Biên do Daydé & Pillé (1898-1903, dài 1 682 m). Trường Tiền do Schneider & Letellier (1897-1900). Bưu điện do kiến trúc sư Marie-Alfred Foulhoux. Hãng Eiffel chỉ mở rộng cầu Trường Tiền năm 1937; Daydé sáp nhập vào tập đoàn Eiffel năm 1964, từ đó sinh ra nhầm lẫn.",
  "lv": 2
 },
 {
  "cat": "Pierres et ponts",
  "fr": "L'Opéra de Hanoï (1911) s'inspire de quel bâtiment parisien ?",
  "vi": "Nhà hát Lớn Hà Nội (1911) lấy cảm hứng từ công trình nào ở Paris?",
  "o": [
   "le Palais Garnier",
   "l'Opéra Bastille",
   "le Grand Palais",
   "le Théâtre du Châtelet"
  ],
  "r": 0,
  "pfr": "Trois fois plus petit que son modèle, il a accueilli en 1946 la première session de l'Assemblée nationale du Vietnam indépendant.",
  "pvi": "Nhỏ hơn bản gốc ba lần, nhưng năm 1946 chính nơi đây diễn ra kỳ họp đầu tiên của Quốc hội nước Việt Nam độc lập.",
  "lv": 1
 },
 {
  "cat": "Pierres et ponts",
  "fr": "À Da Nang, le Pont du Dragon crache le feu et l'eau. Quand ?",
  "vi": "Ở Đà Nẵng, Cầu Rồng phun lửa và phun nước khi nào?",
  "o": [
   "samedi et dimanche à 21 h / thứ Bảy và Chủ nhật lúc 21h",
   "tous les soirs / mỗi tối",
   "le 1er du mois lunaire / mùng 1 âm lịch",
   "au Têt seulement / chỉ dịp Tết"
  ],
  "r": 0,
  "pfr": "Da Nang est la ville des ponts du Vietnam. Sous les Français elle s'appelait Tourane. Le Dragon (666 m) a ouvert le 29 mars 2013, jour anniversaire de la libération de la ville.",
  "pvi": "Đà Nẵng là thành phố của những cây cầu. Thời Pháp thuộc thành phố mang tên Tourane. Cầu Rồng (dài 666 m) khánh thành ngày 29/3/2013, đúng ngày kỷ niệm giải phóng thành phố.",
  "lv": 1
 },
 {
  "cat": "Pierres et ponts",
  "fr": "Le Pont d'Or (Cầu Vàng) de Bà Nà Hills, tenu par deux mains géantes, a fait le tour du monde en 2018. Bà Nà était à l'origine…",
  "vi": "Cầu Vàng ở Bà Nà Hills, được hai bàn tay khổng lồ nâng đỡ, nổi tiếng thế giới năm 2018. Bà Nà ban đầu là gì?",
  "o": [
   "une station climatique française (1919) / khu nghỉ mát của người Pháp (1919)",
   "un monastère / một tu viện",
   "une base militaire / một căn cứ quân sự",
   "une plantation de thé / một đồn điền trà"
  ],
  "r": 0,
  "pfr": "Comme Đà Lạt, Sa Pa et Tam Đảo, Bà Nà a été créée par les Français pour fuir la chaleur. Le pont, lui, est 100 % vietnamien : TA Landscape Architecture, Hồ Chí Minh-Ville.",
  "pvi": "Như Đà Lạt, Sa Pa và Tam Đảo, Bà Nà do người Pháp lập để tránh nóng. Còn cây cầu thì 100 % Việt Nam: TA Landscape Architecture, TP. Hồ Chí Minh.",
  "lv": 1
 },
 {
  "cat": "Pierres et ponts",
  "fr": "La cathédrale Notre-Dame de Saigon (1880) est construite en briques rouges importées. D'où ?",
  "vi": "Nhà thờ Đức Bà Sài Gòn (1880) được xây bằng gạch đỏ nhập khẩu. Từ đâu?",
  "o": [
   "Toulouse",
   "Marseille",
   "Lyon",
   "Hanoï / Hà Nội"
  ],
  "r": 0,
  "pfr": "Tous les matériaux d'origine ont traversé les mers depuis la France. Les briques de Toulouse gardent leur rouge sans peinture depuis 145 ans.",
  "pvi": "Toàn bộ vật liệu ban đầu được chở từ Pháp sang. Gạch Toulouse vẫn giữ màu đỏ không cần sơn suốt 145 năm.",
  "lv": 3
 },
 {
  "cat": "Pierres et ponts",
  "fr": "La ligne de chemin de fer Hanoï–Saigon (1 726 km), achevée en 1936, est aujourd'hui le train de la Réunification. Combien dure le trajet ?",
  "vi": "Tuyến đường sắt Hà Nội–Sài Gòn (1 726 km), hoàn thành năm 1936, nay là tàu Thống Nhất. Hành trình mất bao lâu?",
  "o": [
   "environ 33 h / khoảng 33 giờ",
   "environ 12 h / khoảng 12 giờ",
   "environ 48 h / khoảng 48 giờ",
   "environ 20 h / khoảng 20 giờ"
  ],
  "r": 0,
  "pfr": "Paris–Marseille (750 km de rail) se fait en 3 h de TGV. En 2024, l'Assemblée nationale vietnamienne a approuvé la ligne à grande vitesse Nord-Sud (1 541 km, 350 km/h) : une question pour ingénieurs.",
  "pvi": "Paris–Marseille (750 km đường sắt) chỉ mất 3 giờ bằng TGV. Năm 2024, Quốc hội Việt Nam thông qua dự án đường sắt tốc độ cao Bắc–Nam (1 541 km, 350 km/h): một câu hỏi dành cho kỹ sư.",
  "lv": 2
 },
 {
  "cat": "Géographie miroir",
  "fr": "Fansipan, « toit de l'Indochine », culmine à 3 147 m. Et le Mont Blanc ?",
  "vi": "Fansipan, « nóc nhà Đông Dương », cao 3 147 m. Còn Mont Blanc?",
  "o": [
   "4 806 m",
   "3 900 m",
   "5 200 m",
   "4 200 m"
  ],
  "r": 0,
  "pfr": "Fansipan se fait aujourd'hui en téléphérique (record du monde de dénivelé en 2016). Le Mont Blanc, lui, se mérite encore à pied.",
  "pvi": "Fansipan nay lên bằng cáp treo (kỷ lục thế giới về chênh lệch độ cao, 2016). Mont Blanc thì vẫn phải leo bộ.",
  "lv": 1
 },
 {
  "cat": "Géographie miroir",
  "fr": "Le Vietnam borde 3 pays : Chine, Laos, Cambodge. Avec quel pays la France a-t-elle sa plus longue frontière terrestre ?",
  "vi": "Việt Nam giáp 3 nước: Trung Quốc, Lào, Campuchia. Pháp có đường biên giới trên bộ dài nhất với nước nào?",
  "o": [
   "le Brésil / Brazil",
   "l'Espagne / Tây Ban Nha",
   "la Belgique / Bỉ",
   "l'Allemagne / Đức"
  ],
  "r": 0,
  "pfr": "730 km, grâce à la Guyane, en Amérique du Sud. La France est aussi le pays qui compte le plus de fuseaux horaires au monde : douze.",
  "pvi": "730 km, nhờ Guyane thuộc Pháp ở Nam Mỹ. Pháp cũng là nước có nhiều múi giờ nhất thế giới: mười hai.",
  "lv": 3
 },
 {
  "cat": "Géographie miroir",
  "fr": "Du nord au sud, le Vietnam mesure environ 1 650 km. Et la France métropolitaine, de Dunkerque à Perpignan ?",
  "vi": "Việt Nam dài khoảng 1 650 km từ Bắc vào Nam. Còn nước Pháp lục địa, từ Dunkerque đến Perpignan?",
  "o": [
   "environ 1 000 km / khoảng 1 000 km",
   "environ 1 650 km",
   "environ 2 000 km",
   "environ 700 km"
  ],
  "r": 0,
  "pfr": "Le Vietnam est plus long mais plus petit : 331 000 km² contre 551 000 km². La forme en S contre l'Hexagone.",
  "pvi": "Việt Nam dài hơn nhưng nhỏ hơn: 331 000 km² so với 551 000 km². Hình chữ S so với hình lục giác (l'Hexagone).",
  "lv": 2
 },
 {
  "cat": "Géographie miroir",
  "fr": "Lequel est classé au patrimoine mondial de l'Unesco : la baie d'Hạ Long ou les falaises d'Étretat ?",
  "vi": "Di sản thế giới UNESCO: Vịnh Hạ Long hay vách đá Étretat?",
  "o": [
   "Hạ Long seulement / chỉ Hạ Long",
   "Étretat seulement / chỉ Étretat",
   "les deux / cả hai",
   "aucun / không"
  ],
  "r": 0,
  "pfr": "Hạ Long est classée depuis 1994. Autour de Da Nang, trois sites Unesco à moins de 3 h : Hội An, Mỹ Sơn et Huế. La France en compte plus de 50.",
  "pvi": "Hạ Long được công nhận năm 1994. Quanh Đà Nẵng có ba di sản UNESCO trong vòng 3 giờ: Hội An, Mỹ Sơn và Huế. Pháp có hơn 50 di sản.",
  "lv": 2
 },
 {
  "cat": "Géographie miroir",
  "fr": "Le Mékong (4 350 km) traverse six pays. Quel est le plus long fleuve de France ?",
  "vi": "Sông Mê Kông (4 350 km) chảy qua sáu nước. Sông dài nhất nước Pháp là sông nào?",
  "o": [
   "la Loire (1 006 km)",
   "la Seine",
   "le Rhône",
   "la Garonne"
  ],
  "r": 0,
  "pfr": "La mission Doudart de Lagrée – Francis Garnier a remonté le Mékong de 1866 à 1868. Le Mékong finit au Vietnam en « neuf dragons » (Cửu Long) ; la vallée de la Loire est classée Unesco depuis 2000.",
  "pvi": "Đoàn thám hiểm Doudart de Lagrée – Francis Garnier ngược dòng Mê Kông từ 1866 đến 1868. Mê Kông kết thúc ở Việt Nam bằng « chín con rồng » (Cửu Long); thung lũng sông Loire là di sản UNESCO từ năm 2000.",
  "lv": 1
 },
 {
  "cat": "Géographie miroir",
  "fr": "Quel est le décalage horaire entre Paris et Da Nang ?",
  "vi": "Chênh lệch giờ giữa Paris và Đà Nẵng là bao nhiêu?",
  "o": [
   "5 h en été, 6 h en hiver / 5 giờ mùa hè, 6 giờ mùa đông",
   "7 h toute l'année / 7 giờ quanh năm",
   "6 h en été, 5 h en hiver / 6 giờ hè, 5 giờ đông",
   "4 h / 4 giờ"
  ],
  "r": 0,
  "pfr": "Le Vietnam n'a pas d'heure d'été. Conseil aux étudiants PFIEV : appelez la France après 14 h, heure du Vietnam.",
  "pvi": "Việt Nam không đổi giờ mùa hè. Mẹo cho sinh viên PFIEV: gọi Pháp sau 14h giờ Việt Nam.",
  "lv": 1
 },
 {
  "cat": "Estimation",
  "fr": "Combien de personnes d'origine vietnamienne vivent en France ? (le plus proche gagne)",
  "vi": "Có bao nhiêu người gốc Việt đang sống ở Pháp? (ai gần nhất thắng)",
  "o": [],
  "r": -1,
  "pfr": "Les estimations vont de 300 000 à 400 000 : la plus grande communauté vietnamienne d'Europe, arrivée par vagues depuis les années 1920 et après 1975.",
  "pvi": "Ước tính từ 300 000 đến 400 000: cộng đồng người Việt lớn nhất châu Âu, đến theo nhiều đợt từ những năm 1920 và sau 1975.",
  "num": 350000,
  "lv": 2
 },
 {
  "cat": "Estimation",
  "fr": "Combien de motos sont immatriculées au Vietnam ? (le plus proche gagne)",
  "vi": "Việt Nam có bao nhiêu xe máy đã đăng ký? (ai gần nhất thắng)",
  "o": [],
  "r": -1,
  "pfr": "Environ 75 millions pour 100 millions d'habitants. La France compte environ 4 millions de deux-roues motorisés.",
  "pvi": "Khoảng 75 triệu xe cho 100 triệu dân. Pháp chỉ có khoảng 4 triệu xe hai bánh có động cơ.",
  "num": 75000000,
  "lv": 2
 },
 {
  "cat": "Estimation",
  "fr": "En quelle année a démarré le PFIEV ? (le plus proche gagne)",
  "vi": "PFIEV bắt đầu năm nào? (ai gần nhất thắng)",
  "o": [],
  "r": -1,
  "pfr": "Accord signé en 1997, première promotion en 1999. Quatre universités vietnamiennes, un consortium d'écoles d'ingénieurs françaises, et un diplôme reconnu par la CTI.",
  "pvi": "Hiệp định ký năm 1997, khóa đầu tiên năm 1999. Bốn trường đại học Việt Nam, một liên minh các trường kỹ sư Pháp, và bằng được CTI công nhận.",
  "num": 1999,
  "lv": 2
 },
 {
  "cat": "Histoire partagée",
  "fr": "Les accords de paix de 1973, qui mettent fin à l'engagement américain au Vietnam, ont été signés dans quelle ville ?",
  "vi": "Hiệp định hòa bình năm 1973, chấm dứt sự can dự của Mỹ ở Việt Nam, được ký tại thành phố nào?",
  "o": [
   "Paris",
   "Genève",
   "Washington",
   "Hanoï / Hà Nội"
  ],
  "r": 0,
  "pfr": "Signés le 27 janvier 1973, avenue Kléber. L'autre grand accord, celui de 1954, avait été signé à Genève.",
  "pvi": "Ký ngày 27/1/1973 tại đại lộ Kléber, Paris. Hiệp định lớn còn lại, năm 1954, được ký ở Genève.",
  "lv": 1
 },
 {
  "cat": "Histoire partagée",
  "fr": "En 1920, au congrès de Tours, un Vietnamien de 30 ans figure parmi les fondateurs du Parti communiste français. Qui ?",
  "vi": "Năm 1920, tại Đại hội Tours, một người Việt Nam 30 tuổi là thành viên sáng lập Đảng Cộng sản Pháp. Ông là ai?",
  "o": [
   "Nguyễn Ái Quốc (Hồ Chí Minh)",
   "Phan Châu Trinh",
   "Võ Nguyên Giáp",
   "Phan Bội Châu"
  ],
  "r": 0,
  "pfr": "Un an plus tôt, en 1919, il avait adressé à la conférence de Versailles les « Revendications du peuple annamite ».",
  "pvi": "Một năm trước đó, năm 1919, Người đã gửi « Bản yêu sách của nhân dân An Nam » tới Hội nghị Versailles.",
  "lv": 2
 },
 {
  "cat": "Histoire partagée",
  "fr": "Le chemin de fer à crémaillère de Đà Lạt (1932) a cessé de rouler après 1975. En 1990, ses locomotives ont été vendues à un pays qui les fait encore rouler. Lequel ?",
  "vi": "Đường sắt răng cưa Đà Lạt (1932) ngừng chạy sau 1975. Năm 1990, các đầu máy của nó được bán cho một nước và vẫn chạy đến nay. Nước nào?",
  "o": [
   "la Suisse / Thụy Sĩ",
   "le Japon / Nhật Bản",
   "la France / Pháp",
   "l'Allemagne / Đức"
  ],
  "r": 0,
  "pfr": "Elles roulent sur la ligne de la Furka, dans les Alpes. Le Vietnam veut aujourd'hui rouvrir la ligne Đà Lạt–Tháp Chàm. La Suisse est aussi le pays natal de Yersin, qui a « trouvé » Đà Lạt.",
  "pvi": "Chúng đang chạy trên tuyến Furka trong dãy Alps. Việt Nam nay muốn khôi phục tuyến Đà Lạt–Tháp Chàm. Thụy Sĩ cũng là quê hương của Yersin, người đã « tìm ra » Đà Lạt.",
  "lv": 3
 }
]
)json";
