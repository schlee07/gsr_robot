#ifndef  MOTOR_STEP_TABLE_PRESENT
#define  MOTOR_STEP_TABLE_PRESENT

const unsigned int g_motor_time_table[1000]= {
//[Accelator = 5]
   30699,  12716,   9757,   8225,   7247,   6552,   6025,   5608,   5267,   4981, 
    4738,   4527,   4342,   4178,   4031,   3899,   3779,   3669,   3569,   3476, 
    3390,   3310,   3236,   3166,   3101,   3039,   2981,   2927,   2875,   2826, 
    2779,   2735,   2692,   2652,   2613,   2576,   2540,   2506,   2473,   2442, 
    2412,   2382,   2354,   2327,   2301,   2275,   2251,   2227,   2204,   2181, 
    2160,   2138,   2118,   2098,   2079,   2060,   2042,   2024,   2006,   1989, 
    1973,   1957,   1941,   1926,   1911,   1896,   1882,   1868,   1854,   1841, 
    1828,   1815,   1802,   1790,   1778,   1766,   1755,   1743,   1732,   1721, 
    1710,   1700,   1689,   1679,   1669,   1660,   1650,   1640,   1631,   1622, 
    1613,   1604,   1596,   1587,   1579,   1570,   1562,   1554,   1546,   1538, 
    1531,   1523,   1516,   1508,   1501,   1494,   1487,   1480,   1473,   1466, 
    1460,   1453,   1447,   1440,   1434,   1428,   1422,   1416,   1410,   1404, 
    1398,   1392,   1386,   1381,   1375,   1370,   1364,   1359,   1354,   1348, 
    1343,   1338,   1333,   1328,   1323,   1318,   1313,   1309,   1304,   1299, 
    1294,   1290,   1285,   1281,   1276,   1272,   1268,   1263,   1259,   1255, 
    1251,   1247,   1243,   1238,   1234,   1230,   1227,   1223,   1219,   1215, 
    1211,   1207,   1204,   1200,   1196,   1193,   1189,   1186,   1182,   1179, 
    1175,   1172,   1168,   1165,   1162,   1158,   1155,   1152,   1148,   1145, 
    1142,   1139,   1136,   1133,   1130,   1127,   1124,   1120,   1118,   1115, 
    1112,   1109,   1106,   1103,   1100,   1097,   1095,   1092,   1089,   1086, 
    1084,   1081,   1078,   1076,   1073,   1070,   1068,   1065,   1063,   1060, 
    1057,   1055,   1052,   1050,   1048,   1045,   1043,   1040,   1038,   1036, 
    1033,   1031,   1029,   1026,   1024,   1022,   1019,   1017,   1015,   1013, 
    1011,   1008,   1006,   1004,   1002,   1000,    998,    996,    993,    991, 
     989,    987,    985,    983,    981,    979,    977,    975,    973,    971, 
     969,    967,    965,    964,    962,    960,    958,    956,    954,    952, 
     951,    949,    947,    945,    943,    942,    940,    938,    936,    935, 
     933,    931,    929,    928,    926,    924,    923,    921,    919,    918, 
     916,    914,    913,    911,    910,    908,    906,    905,    903,    902, 
     900,    899,    897,    895,    894,    892,    891,    889,    888,    886, 
     885,    884,    882,    881,    879,    878,    876,    875,    873,    872, 
     871,    869,    868,    866,    865,    864,    862,    861,    860,    858, 
     857,    856,    854,    853,    852,    850,    849,    848,    846,    845, 
     844,    843,    841,    840,    839,    838,    836,    835,    834,    833, 
     831,    830,    829,    828,    827,    825,    824,    823,    822,    821, 
     819,    818,    817,    816,    815,    814,    812,    811,    810,    809, 
     808,    807,    806,    805,    804,    802,    801,    800,    799,    798, 
     797,    796,    795,    794,    793,    792,    791,    790,    788,    787, 
     786,    785,    784,    783,    782,    781,    780,    779,    778,    777, 
     776,    775,    774,    773,    772,    771,    770,    769,    768,    767, 
     767,    766,    765,    764,    763,    762,    761,    760,    759,    758, 
     757,    756,    755,    754,    753,    753,    752,    751,    750,    749, 
     748,    747,    746,    745,    745,    744,    743,    742,    741,    740, 
     739,    738,    738,    737,    736,    735,    734,    733,    733,    732, 
     731,    730,    729,    728,    728,    727,    726,    725,    724,    724, 
     723,    722,    721,    720,    720,    719,    718,    717,    716,    716, 
     715,    714,    713,    712,    712,    711,    710,    709,    709,    708, 
     707,    706,    706,    705,    704,    703,    703,    702,    701,    700, 
     700,    699,    698,    698,    697,    696,    695,    695,    694,    693, 
     693,    692,    691,    690,    690,    689,    688,    688,    687,    686, 
     686,    685,    684,    684,    683,    682,    682,    681,    680,    680, 
     679,    678,    678,    677,    676,    676,    675,    674,    674,    673, 
     672,    672,    671,    670,    670,    669,    668,    668,    667,    667, 
     666,    665,    665,    664,    663,    663,    662,    662,    661,    660, 
     660,    659,    659,    658,    657,    657,    656,    656,    655,    654, 
     654,    653,    653,    652,    651,    651,    650,    650,    649,    648, 
     648,    647,    647,    646,    646,    645,    644,    644,    643,    643, 
     642,    642,    641,    640,    640,    639,    639,    638,    638,    637, 
     637,    636,    636,    635,    634,    634,    633,    633,    632,    632, 
     631,    631,    630,    630,    629,    629,    628,    627,    627,    626, 
     626,    625,    625,    624,    624,    623,    623,    622,    622,    621, 
     621,    620,    620,    619,    619,    618,    618,    617,    617,    616, 
     616,    615,    615,    614,    614,    613,    613,    612,    612,    611, 
     611,    610,    610,    609,    609,    608,    608,    607,    607,    606, 
     606,    606,    605,    605,    604,    604,    603,    603,    602,    602, 
     601,    601,    600,    600,    599,    599,    599,    598,    598,    597, 
     597,    596,    596,    595,    595,    595,    594,    594,    593,    593, 
     592,    592,    591,    591,    591,    590,    590,    589,    589,    588, 
     588,    587,    587,    587,    586,    586,    585,    585,    584,    584, 
     584,    583,    583,    582,    582,    582,    581,    581,    580,    580, 
     579,    579,    579,    578,    578,    577,    577,    577,    576,    576, 
     575,    575,    575,    574,    574,    573,    573,    573,    572,    572, 
     571,    571,    571,    570,    570,    569,    569,    569,    568,    568, 
     567,    567,    567,    566,    566,    565,    565,    565,    564,    564, 
     564,    563,    563,    562,    562,    562,    561,    561,    561,    560, 
     560,    559,    559,    559,    558,    558,    558,    557,    557,    556, 
     556,    556,    555,    555,    555,    554,    554,    554,    553,    553, 
     552,    552,    552,    551,    551,    551,    550,    550,    550,    549, 
     549,    549,    548,    548,    548,    547,    547,    546,    546,    546, 
     545,    545,    545,    544,    544,    544,    543,    543,    543,    542, 
     542,    542,    541,    541,    541,    540,    540,    540,    539,    539, 
     539,    538,    538,    538,    537,    537,    537,    536,    536,    536, 
     535,    535,    535,    534,    534,    534,    533,    533,    533,    532, 
     532,    532,    532,    531,    531,    531,    530,    530,    530,    529, 
     529,    529,    528,    528,    528,    527,    527,    527,    526,    526, 
     526,    526,    525,    525,    525,    524,    524,    524,    523,    523, 
     523,    522,    522,    522,    522,    521,    521,    521,    520,    520, 
     520,    519,    519,    519,    519,    518,    518,    518,    517,    517, 
     517,    517,    516,    516,    516,    515,    515,    515,    514,    514, 
     514,    514,    513,    513,    513,    512,    512,    512,    512,    511, 
     511,    511,    510,    510,    510,    510,    509,    509,    509,    508, 
     508,    508,    508,    507,    507,    507,    507,    506,    506,    506, 
     505,    505,    505,    505,    504,    504,    504,    504,    503,    503, 
     503,    502,    502,    502,    502,    501,    501,    501,    501,    500, 
     500,    500,    499,    499,    499,    499,    498,    498,    498,    498, 
     497,    497,    497,    497,    496,    496,    496,    496,    495,    495, 
     495,    495,    494,    494,    494,    494,    493,    493,    493,    492, 
     492,    492,    492,    491,    491,    491,    491,    490,    490,    490, 
     490,    489,    489,    489,    489,    488,    488,    488,    488,    487, 
     487,    487,    487,    486,    486,    486,    486,    486,    485,    485, 

 };




#endif /* MOTOR_STEP_TABLE_PRESENT*/

