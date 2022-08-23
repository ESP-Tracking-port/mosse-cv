namespace Mosse {
constexpr const unsigned kHann50x50Height = 50;  // Number of rows
constexpr const unsigned kHann50x50Width = 50;  // Number of columns
constexpr const float kHann50x50[kHann50x50Height][kHann50x50Width] = {
	{0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, },
	{0.0000, 0.0000, 0.0001, 0.0002, 0.0003, 0.0004, 0.0006, 0.0008, 0.0010, 0.0012, 0.0015, 0.0017, 0.0020, 0.0022, 0.0025, 0.0028, 0.0030, 0.0032, 0.0034, 0.0036, 0.0038, 0.0039, 0.0040, 0.0041, 0.0041, 0.0041, 0.0041, 0.0040, 0.0039, 0.0038, 0.0036, 0.0034, 0.0032, 0.0030, 0.0028, 0.0025, 0.0022, 0.0020, 0.0017, 0.0015, 0.0012, 0.0010, 0.0008, 0.0006, 0.0004, 0.0003, 0.0002, 0.0001, 0.0000, 0.0000, },
	{0.0000, 0.0001, 0.0003, 0.0006, 0.0011, 0.0016, 0.0023, 0.0031, 0.0039, 0.0049, 0.0058, 0.0069, 0.0079, 0.0090, 0.0100, 0.0110, 0.0120, 0.0129, 0.0137, 0.0144, 0.0150, 0.0155, 0.0159, 0.0162, 0.0163, 0.0163, 0.0162, 0.0159, 0.0155, 0.0150, 0.0144, 0.0137, 0.0129, 0.0120, 0.0110, 0.0100, 0.0090, 0.0079, 0.0069, 0.0058, 0.0049, 0.0039, 0.0031, 0.0023, 0.0016, 0.0011, 0.0006, 0.0003, 0.0001, 0.0000, },
	{0.0000, 0.0002, 0.0006, 0.0013, 0.0024, 0.0036, 0.0051, 0.0069, 0.0088, 0.0109, 0.0131, 0.0154, 0.0177, 0.0200, 0.0223, 0.0246, 0.0267, 0.0287, 0.0306, 0.0322, 0.0336, 0.0347, 0.0356, 0.0362, 0.0365, 0.0365, 0.0362, 0.0356, 0.0347, 0.0336, 0.0322, 0.0306, 0.0287, 0.0267, 0.0246, 0.0223, 0.0200, 0.0177, 0.0154, 0.0131, 0.0109, 0.0088, 0.0069, 0.0051, 0.0036, 0.0024, 0.0013, 0.0006, 0.0002, 0.0000, },
	{0.0000, 0.0003, 0.0011, 0.0024, 0.0041, 0.0064, 0.0091, 0.0121, 0.0155, 0.0191, 0.0230, 0.0270, 0.0311, 0.0353, 0.0393, 0.0433, 0.0471, 0.0506, 0.0538, 0.0567, 0.0591, 0.0612, 0.0627, 0.0637, 0.0643, 0.0643, 0.0637, 0.0627, 0.0612, 0.0591, 0.0567, 0.0538, 0.0506, 0.0471, 0.0433, 0.0393, 0.0353, 0.0311, 0.0270, 0.0230, 0.0191, 0.0155, 0.0121, 0.0091, 0.0064, 0.0041, 0.0024, 0.0011, 0.0003, 0.0000, },
	{0.0000, 0.0004, 0.0016, 0.0036, 0.0064, 0.0099, 0.0140, 0.0187, 0.0239, 0.0296, 0.0355, 0.0417, 0.0481, 0.0544, 0.0607, 0.0668, 0.0726, 0.0781, 0.0830, 0.0874, 0.0913, 0.0944, 0.0968, 0.0984, 0.0992, 0.0992, 0.0984, 0.0968, 0.0944, 0.0913, 0.0874, 0.0830, 0.0781, 0.0726, 0.0668, 0.0607, 0.0544, 0.0481, 0.0417, 0.0355, 0.0296, 0.0239, 0.0187, 0.0140, 0.0099, 0.0064, 0.0036, 0.0016, 0.0004, 0.0000, },
	{0.0000, 0.0006, 0.0023, 0.0051, 0.0091, 0.0140, 0.0198, 0.0265, 0.0339, 0.0419, 0.0504, 0.0592, 0.0682, 0.0772, 0.0861, 0.0947, 0.1030, 0.1107, 0.1178, 0.1240, 0.1294, 0.1339, 0.1372, 0.1395, 0.1407, 0.1407, 0.1395, 0.1372, 0.1339, 0.1294, 0.1240, 0.1178, 0.1107, 0.1030, 0.0947, 0.0861, 0.0772, 0.0682, 0.0592, 0.0504, 0.0419, 0.0339, 0.0265, 0.0198, 0.0140, 0.0091, 0.0051, 0.0023, 0.0006, 0.0000, },
	{0.0000, 0.0008, 0.0031, 0.0069, 0.0121, 0.0187, 0.0265, 0.0354, 0.0453, 0.0560, 0.0673, 0.0791, 0.0911, 0.1032, 0.1151, 0.1266, 0.1377, 0.1480, 0.1574, 0.1658, 0.1730, 0.1789, 0.1835, 0.1865, 0.1881, 0.1881, 0.1865, 0.1835, 0.1789, 0.1730, 0.1658, 0.1574, 0.1480, 0.1377, 0.1266, 0.1151, 0.1032, 0.0911, 0.0791, 0.0673, 0.0560, 0.0453, 0.0354, 0.0265, 0.0187, 0.0121, 0.0069, 0.0031, 0.0008, 0.0000, },
	{0.0000, 0.0010, 0.0039, 0.0088, 0.0155, 0.0239, 0.0339, 0.0453, 0.0580, 0.0717, 0.0861, 0.1012, 0.1165, 0.1320, 0.1472, 0.1620, 0.1761, 0.1893, 0.2013, 0.2121, 0.2213, 0.2289, 0.2347, 0.2386, 0.2406, 0.2406, 0.2386, 0.2347, 0.2289, 0.2213, 0.2121, 0.2013, 0.1893, 0.1761, 0.1620, 0.1472, 0.1320, 0.1165, 0.1012, 0.0861, 0.0717, 0.0580, 0.0453, 0.0339, 0.0239, 0.0155, 0.0088, 0.0039, 0.0010, 0.0000, },
	{0.0000, 0.0012, 0.0049, 0.0109, 0.0191, 0.0296, 0.0419, 0.0560, 0.0717, 0.0886, 0.1065, 0.1251, 0.1440, 0.1631, 0.1819, 0.2002, 0.2176, 0.2339, 0.2488, 0.2621, 0.2735, 0.2829, 0.2900, 0.2949, 0.2973, 0.2973, 0.2949, 0.2900, 0.2829, 0.2735, 0.2621, 0.2488, 0.2339, 0.2176, 0.2002, 0.1819, 0.1631, 0.1440, 0.1251, 0.1065, 0.0886, 0.0717, 0.0560, 0.0419, 0.0296, 0.0191, 0.0109, 0.0049, 0.0012, 0.0000, },
	{0.0000, 0.0015, 0.0058, 0.0131, 0.0230, 0.0355, 0.0504, 0.0673, 0.0861, 0.1065, 0.1280, 0.1503, 0.1731, 0.1960, 0.2187, 0.2406, 0.2616, 0.2812, 0.2991, 0.3151, 0.3288, 0.3400, 0.3486, 0.3544, 0.3574, 0.3574, 0.3544, 0.3486, 0.3400, 0.3288, 0.3151, 0.2991, 0.2812, 0.2616, 0.2406, 0.2187, 0.1960, 0.1731, 0.1503, 0.1280, 0.1065, 0.0861, 0.0673, 0.0504, 0.0355, 0.0230, 0.0131, 0.0058, 0.0015, 0.0000, },
	{0.0000, 0.0017, 0.0069, 0.0154, 0.0270, 0.0417, 0.0592, 0.0791, 0.1012, 0.1251, 0.1503, 0.1766, 0.2034, 0.2303, 0.2569, 0.2827, 0.3073, 0.3303, 0.3514, 0.3701, 0.3862, 0.3994, 0.4095, 0.4163, 0.4198, 0.4198, 0.4163, 0.4095, 0.3994, 0.3862, 0.3701, 0.3514, 0.3303, 0.3073, 0.2827, 0.2569, 0.2303, 0.2034, 0.1766, 0.1503, 0.1251, 0.1012, 0.0791, 0.0592, 0.0417, 0.0270, 0.0154, 0.0069, 0.0017, 0.0000, },
	{0.0000, 0.0020, 0.0079, 0.0177, 0.0311, 0.0481, 0.0682, 0.0911, 0.1165, 0.1440, 0.1731, 0.2034, 0.2342, 0.2652, 0.2958, 0.3256, 0.3539, 0.3804, 0.4047, 0.4262, 0.4448, 0.4600, 0.4716, 0.4795, 0.4835, 0.4835, 0.4795, 0.4716, 0.4600, 0.4448, 0.4262, 0.4047, 0.3804, 0.3539, 0.3256, 0.2958, 0.2652, 0.2342, 0.2034, 0.1731, 0.1440, 0.1165, 0.0911, 0.0682, 0.0481, 0.0311, 0.0177, 0.0079, 0.0020, 0.0000, },
	{0.0000, 0.0022, 0.0090, 0.0200, 0.0353, 0.0544, 0.0772, 0.1032, 0.1320, 0.1631, 0.1960, 0.2303, 0.2652, 0.3003, 0.3350, 0.3686, 0.4007, 0.4308, 0.4582, 0.4826, 0.5036, 0.5209, 0.5341, 0.5430, 0.5474, 0.5474, 0.5430, 0.5341, 0.5209, 0.5036, 0.4826, 0.4582, 0.4308, 0.4007, 0.3686, 0.3350, 0.3003, 0.2652, 0.2303, 0.1960, 0.1631, 0.1320, 0.1032, 0.0772, 0.0544, 0.0353, 0.0200, 0.0090, 0.0022, 0.0000, },
	{0.0000, 0.0025, 0.0100, 0.0223, 0.0393, 0.0607, 0.0861, 0.1151, 0.1472, 0.1819, 0.2187, 0.2569, 0.2958, 0.3350, 0.3736, 0.4112, 0.4470, 0.4805, 0.5111, 0.5384, 0.5618, 0.5810, 0.5957, 0.6056, 0.6106, 0.6106, 0.6056, 0.5957, 0.5810, 0.5618, 0.5384, 0.5111, 0.4805, 0.4470, 0.4112, 0.3736, 0.3350, 0.2958, 0.2569, 0.2187, 0.1819, 0.1472, 0.1151, 0.0861, 0.0607, 0.0393, 0.0223, 0.0100, 0.0025, 0.0000, },
	{0.0000, 0.0028, 0.0110, 0.0246, 0.0433, 0.0668, 0.0947, 0.1266, 0.1620, 0.2002, 0.2406, 0.2827, 0.3256, 0.3686, 0.4112, 0.4525, 0.4919, 0.5288, 0.5625, 0.5924, 0.6182, 0.6394, 0.6555, 0.6665, 0.6720, 0.6720, 0.6665, 0.6555, 0.6394, 0.6182, 0.5924, 0.5625, 0.5288, 0.4919, 0.4525, 0.4112, 0.3686, 0.3256, 0.2827, 0.2406, 0.2002, 0.1620, 0.1266, 0.0947, 0.0668, 0.0433, 0.0246, 0.0110, 0.0028, 0.0000, },
	{0.0000, 0.0030, 0.0120, 0.0267, 0.0471, 0.0726, 0.1030, 0.1377, 0.1761, 0.2176, 0.2616, 0.3073, 0.3539, 0.4007, 0.4470, 0.4919, 0.5348, 0.5748, 0.6115, 0.6440, 0.6721, 0.6951, 0.7126, 0.7245, 0.7305, 0.7305, 0.7245, 0.7126, 0.6951, 0.6721, 0.6440, 0.6115, 0.5748, 0.5348, 0.4919, 0.4470, 0.4007, 0.3539, 0.3073, 0.2616, 0.2176, 0.1761, 0.1377, 0.1030, 0.0726, 0.0471, 0.0267, 0.0120, 0.0030, 0.0000, },
	{0.0000, 0.0032, 0.0129, 0.0287, 0.0506, 0.0781, 0.1107, 0.1480, 0.1893, 0.2339, 0.2812, 0.3303, 0.3804, 0.4308, 0.4805, 0.5288, 0.5748, 0.6179, 0.6573, 0.6923, 0.7224, 0.7471, 0.7660, 0.7788, 0.7853, 0.7853, 0.7788, 0.7660, 0.7471, 0.7224, 0.6923, 0.6573, 0.6179, 0.5748, 0.5288, 0.4805, 0.4308, 0.3804, 0.3303, 0.2812, 0.2339, 0.1893, 0.1480, 0.1107, 0.0781, 0.0506, 0.0287, 0.0129, 0.0032, 0.0000, },
	{0.0000, 0.0034, 0.0137, 0.0306, 0.0538, 0.0830, 0.1178, 0.1574, 0.2013, 0.2488, 0.2991, 0.3514, 0.4047, 0.4582, 0.5111, 0.5625, 0.6115, 0.6573, 0.6991, 0.7364, 0.7685, 0.7947, 0.8149, 0.8284, 0.8353, 0.8353, 0.8284, 0.8149, 0.7947, 0.7685, 0.7364, 0.6991, 0.6573, 0.6115, 0.5625, 0.5111, 0.4582, 0.4047, 0.3514, 0.2991, 0.2488, 0.2013, 0.1574, 0.1178, 0.0830, 0.0538, 0.0306, 0.0137, 0.0034, 0.0000, },
	{0.0000, 0.0036, 0.0144, 0.0322, 0.0567, 0.0874, 0.1240, 0.1658, 0.2121, 0.2621, 0.3151, 0.3701, 0.4262, 0.4826, 0.5384, 0.5924, 0.6440, 0.6923, 0.7364, 0.7757, 0.8094, 0.8371, 0.8583, 0.8726, 0.8798, 0.8798, 0.8726, 0.8583, 0.8371, 0.8094, 0.7757, 0.7364, 0.6923, 0.6440, 0.5924, 0.5384, 0.4826, 0.4262, 0.3701, 0.3151, 0.2621, 0.2121, 0.1658, 0.1240, 0.0874, 0.0567, 0.0322, 0.0144, 0.0036, 0.0000, },
	{0.0000, 0.0038, 0.0150, 0.0336, 0.0591, 0.0913, 0.1294, 0.1730, 0.2213, 0.2735, 0.3288, 0.3862, 0.4448, 0.5036, 0.5618, 0.6182, 0.6721, 0.7224, 0.7685, 0.8094, 0.8446, 0.8735, 0.8956, 0.9106, 0.9181, 0.9181, 0.9106, 0.8956, 0.8735, 0.8446, 0.8094, 0.7685, 0.7224, 0.6721, 0.6182, 0.5618, 0.5036, 0.4448, 0.3862, 0.3288, 0.2735, 0.2213, 0.1730, 0.1294, 0.0913, 0.0591, 0.0336, 0.0150, 0.0038, 0.0000, },
	{0.0000, 0.0039, 0.0155, 0.0347, 0.0612, 0.0944, 0.1339, 0.1789, 0.2289, 0.2829, 0.3400, 0.3994, 0.4600, 0.5209, 0.5810, 0.6394, 0.6951, 0.7471, 0.7947, 0.8371, 0.8735, 0.9034, 0.9263, 0.9417, 0.9495, 0.9495, 0.9417, 0.9263, 0.9034, 0.8735, 0.8371, 0.7947, 0.7471, 0.6951, 0.6394, 0.5810, 0.5209, 0.4600, 0.3994, 0.3400, 0.2829, 0.2289, 0.1789, 0.1339, 0.0944, 0.0612, 0.0347, 0.0155, 0.0039, 0.0000, },
	{0.0000, 0.0040, 0.0159, 0.0356, 0.0627, 0.0968, 0.1372, 0.1835, 0.2347, 0.2900, 0.3486, 0.4095, 0.4716, 0.5341, 0.5957, 0.6555, 0.7126, 0.7660, 0.8149, 0.8583, 0.8956, 0.9263, 0.9497, 0.9655, 0.9735, 0.9735, 0.9655, 0.9497, 0.9263, 0.8956, 0.8583, 0.8149, 0.7660, 0.7126, 0.6555, 0.5957, 0.5341, 0.4716, 0.4095, 0.3486, 0.2900, 0.2347, 0.1835, 0.1372, 0.0968, 0.0627, 0.0356, 0.0159, 0.0040, 0.0000, },
	{0.0000, 0.0041, 0.0162, 0.0362, 0.0637, 0.0984, 0.1395, 0.1865, 0.2386, 0.2949, 0.3544, 0.4163, 0.4795, 0.5430, 0.6056, 0.6665, 0.7245, 0.7788, 0.8284, 0.8726, 0.9106, 0.9417, 0.9655, 0.9816, 0.9898, 0.9898, 0.9816, 0.9655, 0.9417, 0.9106, 0.8726, 0.8284, 0.7788, 0.7245, 0.6665, 0.6056, 0.5430, 0.4795, 0.4163, 0.3544, 0.2949, 0.2386, 0.1865, 0.1395, 0.0984, 0.0637, 0.0362, 0.0162, 0.0041, 0.0000, },
	{0.0000, 0.0041, 0.0163, 0.0365, 0.0643, 0.0992, 0.1407, 0.1881, 0.2406, 0.2973, 0.3574, 0.4198, 0.4835, 0.5474, 0.6106, 0.6720, 0.7305, 0.7853, 0.8353, 0.8798, 0.9181, 0.9495, 0.9735, 0.9898, 0.9979, 0.9979, 0.9898, 0.9735, 0.9495, 0.9181, 0.8798, 0.8353, 0.7853, 0.7305, 0.6720, 0.6106, 0.5474, 0.4835, 0.4198, 0.3574, 0.2973, 0.2406, 0.1881, 0.1407, 0.0992, 0.0643, 0.0365, 0.0163, 0.0041, 0.0000, },
	{0.0000, 0.0041, 0.0163, 0.0365, 0.0643, 0.0992, 0.1407, 0.1881, 0.2406, 0.2973, 0.3574, 0.4198, 0.4835, 0.5474, 0.6106, 0.6720, 0.7305, 0.7853, 0.8353, 0.8798, 0.9181, 0.9495, 0.9735, 0.9898, 0.9979, 0.9979, 0.9898, 0.9735, 0.9495, 0.9181, 0.8798, 0.8353, 0.7853, 0.7305, 0.6720, 0.6106, 0.5474, 0.4835, 0.4198, 0.3574, 0.2973, 0.2406, 0.1881, 0.1407, 0.0992, 0.0643, 0.0365, 0.0163, 0.0041, 0.0000, },
	{0.0000, 0.0041, 0.0162, 0.0362, 0.0637, 0.0984, 0.1395, 0.1865, 0.2386, 0.2949, 0.3544, 0.4163, 0.4795, 0.5430, 0.6056, 0.6665, 0.7245, 0.7788, 0.8284, 0.8726, 0.9106, 0.9417, 0.9655, 0.9816, 0.9898, 0.9898, 0.9816, 0.9655, 0.9417, 0.9106, 0.8726, 0.8284, 0.7788, 0.7245, 0.6665, 0.6056, 0.5430, 0.4795, 0.4163, 0.3544, 0.2949, 0.2386, 0.1865, 0.1395, 0.0984, 0.0637, 0.0362, 0.0162, 0.0041, 0.0000, },
	{0.0000, 0.0040, 0.0159, 0.0356, 0.0627, 0.0968, 0.1372, 0.1835, 0.2347, 0.2900, 0.3486, 0.4095, 0.4716, 0.5341, 0.5957, 0.6555, 0.7126, 0.7660, 0.8149, 0.8583, 0.8956, 0.9263, 0.9497, 0.9655, 0.9735, 0.9735, 0.9655, 0.9497, 0.9263, 0.8956, 0.8583, 0.8149, 0.7660, 0.7126, 0.6555, 0.5957, 0.5341, 0.4716, 0.4095, 0.3486, 0.2900, 0.2347, 0.1835, 0.1372, 0.0968, 0.0627, 0.0356, 0.0159, 0.0040, 0.0000, },
	{0.0000, 0.0039, 0.0155, 0.0347, 0.0612, 0.0944, 0.1339, 0.1789, 0.2289, 0.2829, 0.3400, 0.3994, 0.4600, 0.5209, 0.5810, 0.6394, 0.6951, 0.7471, 0.7947, 0.8371, 0.8735, 0.9034, 0.9263, 0.9417, 0.9495, 0.9495, 0.9417, 0.9263, 0.9034, 0.8735, 0.8371, 0.7947, 0.7471, 0.6951, 0.6394, 0.5810, 0.5209, 0.4600, 0.3994, 0.3400, 0.2829, 0.2289, 0.1789, 0.1339, 0.0944, 0.0612, 0.0347, 0.0155, 0.0039, 0.0000, },
	{0.0000, 0.0038, 0.0150, 0.0336, 0.0591, 0.0913, 0.1294, 0.1730, 0.2213, 0.2735, 0.3288, 0.3862, 0.4448, 0.5036, 0.5618, 0.6182, 0.6721, 0.7224, 0.7685, 0.8094, 0.8446, 0.8735, 0.8956, 0.9106, 0.9181, 0.9181, 0.9106, 0.8956, 0.8735, 0.8446, 0.8094, 0.7685, 0.7224, 0.6721, 0.6182, 0.5618, 0.5036, 0.4448, 0.3862, 0.3288, 0.2735, 0.2213, 0.1730, 0.1294, 0.0913, 0.0591, 0.0336, 0.0150, 0.0038, 0.0000, },
	{0.0000, 0.0036, 0.0144, 0.0322, 0.0567, 0.0874, 0.1240, 0.1658, 0.2121, 0.2621, 0.3151, 0.3701, 0.4262, 0.4826, 0.5384, 0.5924, 0.6440, 0.6923, 0.7364, 0.7757, 0.8094, 0.8371, 0.8583, 0.8726, 0.8798, 0.8798, 0.8726, 0.8583, 0.8371, 0.8094, 0.7757, 0.7364, 0.6923, 0.6440, 0.5924, 0.5384, 0.4826, 0.4262, 0.3701, 0.3151, 0.2621, 0.2121, 0.1658, 0.1240, 0.0874, 0.0567, 0.0322, 0.0144, 0.0036, 0.0000, },
	{0.0000, 0.0034, 0.0137, 0.0306, 0.0538, 0.0830, 0.1178, 0.1574, 0.2013, 0.2488, 0.2991, 0.3514, 0.4047, 0.4582, 0.5111, 0.5625, 0.6115, 0.6573, 0.6991, 0.7364, 0.7685, 0.7947, 0.8149, 0.8284, 0.8353, 0.8353, 0.8284, 0.8149, 0.7947, 0.7685, 0.7364, 0.6991, 0.6573, 0.6115, 0.5625, 0.5111, 0.4582, 0.4047, 0.3514, 0.2991, 0.2488, 0.2013, 0.1574, 0.1178, 0.0830, 0.0538, 0.0306, 0.0137, 0.0034, 0.0000, },
	{0.0000, 0.0032, 0.0129, 0.0287, 0.0506, 0.0781, 0.1107, 0.1480, 0.1893, 0.2339, 0.2812, 0.3303, 0.3804, 0.4308, 0.4805, 0.5288, 0.5748, 0.6179, 0.6573, 0.6923, 0.7224, 0.7471, 0.7660, 0.7788, 0.7853, 0.7853, 0.7788, 0.7660, 0.7471, 0.7224, 0.6923, 0.6573, 0.6179, 0.5748, 0.5288, 0.4805, 0.4308, 0.3804, 0.3303, 0.2812, 0.2339, 0.1893, 0.1480, 0.1107, 0.0781, 0.0506, 0.0287, 0.0129, 0.0032, 0.0000, },
	{0.0000, 0.0030, 0.0120, 0.0267, 0.0471, 0.0726, 0.1030, 0.1377, 0.1761, 0.2176, 0.2616, 0.3073, 0.3539, 0.4007, 0.4470, 0.4919, 0.5348, 0.5748, 0.6115, 0.6440, 0.6721, 0.6951, 0.7126, 0.7245, 0.7305, 0.7305, 0.7245, 0.7126, 0.6951, 0.6721, 0.6440, 0.6115, 0.5748, 0.5348, 0.4919, 0.4470, 0.4007, 0.3539, 0.3073, 0.2616, 0.2176, 0.1761, 0.1377, 0.1030, 0.0726, 0.0471, 0.0267, 0.0120, 0.0030, 0.0000, },
	{0.0000, 0.0028, 0.0110, 0.0246, 0.0433, 0.0668, 0.0947, 0.1266, 0.1620, 0.2002, 0.2406, 0.2827, 0.3256, 0.3686, 0.4112, 0.4525, 0.4919, 0.5288, 0.5625, 0.5924, 0.6182, 0.6394, 0.6555, 0.6665, 0.6720, 0.6720, 0.6665, 0.6555, 0.6394, 0.6182, 0.5924, 0.5625, 0.5288, 0.4919, 0.4525, 0.4112, 0.3686, 0.3256, 0.2827, 0.2406, 0.2002, 0.1620, 0.1266, 0.0947, 0.0668, 0.0433, 0.0246, 0.0110, 0.0028, 0.0000, },
	{0.0000, 0.0025, 0.0100, 0.0223, 0.0393, 0.0607, 0.0861, 0.1151, 0.1472, 0.1819, 0.2187, 0.2569, 0.2958, 0.3350, 0.3736, 0.4112, 0.4470, 0.4805, 0.5111, 0.5384, 0.5618, 0.5810, 0.5957, 0.6056, 0.6106, 0.6106, 0.6056, 0.5957, 0.5810, 0.5618, 0.5384, 0.5111, 0.4805, 0.4470, 0.4112, 0.3736, 0.3350, 0.2958, 0.2569, 0.2187, 0.1819, 0.1472, 0.1151, 0.0861, 0.0607, 0.0393, 0.0223, 0.0100, 0.0025, 0.0000, },
	{0.0000, 0.0022, 0.0090, 0.0200, 0.0353, 0.0544, 0.0772, 0.1032, 0.1320, 0.1631, 0.1960, 0.2303, 0.2652, 0.3003, 0.3350, 0.3686, 0.4007, 0.4308, 0.4582, 0.4826, 0.5036, 0.5209, 0.5341, 0.5430, 0.5474, 0.5474, 0.5430, 0.5341, 0.5209, 0.5036, 0.4826, 0.4582, 0.4308, 0.4007, 0.3686, 0.3350, 0.3003, 0.2652, 0.2303, 0.1960, 0.1631, 0.1320, 0.1032, 0.0772, 0.0544, 0.0353, 0.0200, 0.0090, 0.0022, 0.0000, },
	{0.0000, 0.0020, 0.0079, 0.0177, 0.0311, 0.0481, 0.0682, 0.0911, 0.1165, 0.1440, 0.1731, 0.2034, 0.2342, 0.2652, 0.2958, 0.3256, 0.3539, 0.3804, 0.4047, 0.4262, 0.4448, 0.4600, 0.4716, 0.4795, 0.4835, 0.4835, 0.4795, 0.4716, 0.4600, 0.4448, 0.4262, 0.4047, 0.3804, 0.3539, 0.3256, 0.2958, 0.2652, 0.2342, 0.2034, 0.1731, 0.1440, 0.1165, 0.0911, 0.0682, 0.0481, 0.0311, 0.0177, 0.0079, 0.0020, 0.0000, },
	{0.0000, 0.0017, 0.0069, 0.0154, 0.0270, 0.0417, 0.0592, 0.0791, 0.1012, 0.1251, 0.1503, 0.1766, 0.2034, 0.2303, 0.2569, 0.2827, 0.3073, 0.3303, 0.3514, 0.3701, 0.3862, 0.3994, 0.4095, 0.4163, 0.4198, 0.4198, 0.4163, 0.4095, 0.3994, 0.3862, 0.3701, 0.3514, 0.3303, 0.3073, 0.2827, 0.2569, 0.2303, 0.2034, 0.1766, 0.1503, 0.1251, 0.1012, 0.0791, 0.0592, 0.0417, 0.0270, 0.0154, 0.0069, 0.0017, 0.0000, },
	{0.0000, 0.0015, 0.0058, 0.0131, 0.0230, 0.0355, 0.0504, 0.0673, 0.0861, 0.1065, 0.1280, 0.1503, 0.1731, 0.1960, 0.2187, 0.2406, 0.2616, 0.2812, 0.2991, 0.3151, 0.3288, 0.3400, 0.3486, 0.3544, 0.3574, 0.3574, 0.3544, 0.3486, 0.3400, 0.3288, 0.3151, 0.2991, 0.2812, 0.2616, 0.2406, 0.2187, 0.1960, 0.1731, 0.1503, 0.1280, 0.1065, 0.0861, 0.0673, 0.0504, 0.0355, 0.0230, 0.0131, 0.0058, 0.0015, 0.0000, },
	{0.0000, 0.0012, 0.0049, 0.0109, 0.0191, 0.0296, 0.0419, 0.0560, 0.0717, 0.0886, 0.1065, 0.1251, 0.1440, 0.1631, 0.1819, 0.2002, 0.2176, 0.2339, 0.2488, 0.2621, 0.2735, 0.2829, 0.2900, 0.2949, 0.2973, 0.2973, 0.2949, 0.2900, 0.2829, 0.2735, 0.2621, 0.2488, 0.2339, 0.2176, 0.2002, 0.1819, 0.1631, 0.1440, 0.1251, 0.1065, 0.0886, 0.0717, 0.0560, 0.0419, 0.0296, 0.0191, 0.0109, 0.0049, 0.0012, 0.0000, },
	{0.0000, 0.0010, 0.0039, 0.0088, 0.0155, 0.0239, 0.0339, 0.0453, 0.0580, 0.0717, 0.0861, 0.1012, 0.1165, 0.1320, 0.1472, 0.1620, 0.1761, 0.1893, 0.2013, 0.2121, 0.2213, 0.2289, 0.2347, 0.2386, 0.2406, 0.2406, 0.2386, 0.2347, 0.2289, 0.2213, 0.2121, 0.2013, 0.1893, 0.1761, 0.1620, 0.1472, 0.1320, 0.1165, 0.1012, 0.0861, 0.0717, 0.0580, 0.0453, 0.0339, 0.0239, 0.0155, 0.0088, 0.0039, 0.0010, 0.0000, },
	{0.0000, 0.0008, 0.0031, 0.0069, 0.0121, 0.0187, 0.0265, 0.0354, 0.0453, 0.0560, 0.0673, 0.0791, 0.0911, 0.1032, 0.1151, 0.1266, 0.1377, 0.1480, 0.1574, 0.1658, 0.1730, 0.1789, 0.1835, 0.1865, 0.1881, 0.1881, 0.1865, 0.1835, 0.1789, 0.1730, 0.1658, 0.1574, 0.1480, 0.1377, 0.1266, 0.1151, 0.1032, 0.0911, 0.0791, 0.0673, 0.0560, 0.0453, 0.0354, 0.0265, 0.0187, 0.0121, 0.0069, 0.0031, 0.0008, 0.0000, },
	{0.0000, 0.0006, 0.0023, 0.0051, 0.0091, 0.0140, 0.0198, 0.0265, 0.0339, 0.0419, 0.0504, 0.0592, 0.0682, 0.0772, 0.0861, 0.0947, 0.1030, 0.1107, 0.1178, 0.1240, 0.1294, 0.1339, 0.1372, 0.1395, 0.1407, 0.1407, 0.1395, 0.1372, 0.1339, 0.1294, 0.1240, 0.1178, 0.1107, 0.1030, 0.0947, 0.0861, 0.0772, 0.0682, 0.0592, 0.0504, 0.0419, 0.0339, 0.0265, 0.0198, 0.0140, 0.0091, 0.0051, 0.0023, 0.0006, 0.0000, },
	{0.0000, 0.0004, 0.0016, 0.0036, 0.0064, 0.0099, 0.0140, 0.0187, 0.0239, 0.0296, 0.0355, 0.0417, 0.0481, 0.0544, 0.0607, 0.0668, 0.0726, 0.0781, 0.0830, 0.0874, 0.0913, 0.0944, 0.0968, 0.0984, 0.0992, 0.0992, 0.0984, 0.0968, 0.0944, 0.0913, 0.0874, 0.0830, 0.0781, 0.0726, 0.0668, 0.0607, 0.0544, 0.0481, 0.0417, 0.0355, 0.0296, 0.0239, 0.0187, 0.0140, 0.0099, 0.0064, 0.0036, 0.0016, 0.0004, 0.0000, },
	{0.0000, 0.0003, 0.0011, 0.0024, 0.0041, 0.0064, 0.0091, 0.0121, 0.0155, 0.0191, 0.0230, 0.0270, 0.0311, 0.0353, 0.0393, 0.0433, 0.0471, 0.0506, 0.0538, 0.0567, 0.0591, 0.0612, 0.0627, 0.0637, 0.0643, 0.0643, 0.0637, 0.0627, 0.0612, 0.0591, 0.0567, 0.0538, 0.0506, 0.0471, 0.0433, 0.0393, 0.0353, 0.0311, 0.0270, 0.0230, 0.0191, 0.0155, 0.0121, 0.0091, 0.0064, 0.0041, 0.0024, 0.0011, 0.0003, 0.0000, },
	{0.0000, 0.0002, 0.0006, 0.0013, 0.0024, 0.0036, 0.0051, 0.0069, 0.0088, 0.0109, 0.0131, 0.0154, 0.0177, 0.0200, 0.0223, 0.0246, 0.0267, 0.0287, 0.0306, 0.0322, 0.0336, 0.0347, 0.0356, 0.0362, 0.0365, 0.0365, 0.0362, 0.0356, 0.0347, 0.0336, 0.0322, 0.0306, 0.0287, 0.0267, 0.0246, 0.0223, 0.0200, 0.0177, 0.0154, 0.0131, 0.0109, 0.0088, 0.0069, 0.0051, 0.0036, 0.0024, 0.0013, 0.0006, 0.0002, 0.0000, },
	{0.0000, 0.0001, 0.0003, 0.0006, 0.0011, 0.0016, 0.0023, 0.0031, 0.0039, 0.0049, 0.0058, 0.0069, 0.0079, 0.0090, 0.0100, 0.0110, 0.0120, 0.0129, 0.0137, 0.0144, 0.0150, 0.0155, 0.0159, 0.0162, 0.0163, 0.0163, 0.0162, 0.0159, 0.0155, 0.0150, 0.0144, 0.0137, 0.0129, 0.0120, 0.0110, 0.0100, 0.0090, 0.0079, 0.0069, 0.0058, 0.0049, 0.0039, 0.0031, 0.0023, 0.0016, 0.0011, 0.0006, 0.0003, 0.0001, 0.0000, },
	{0.0000, 0.0000, 0.0001, 0.0002, 0.0003, 0.0004, 0.0006, 0.0008, 0.0010, 0.0012, 0.0015, 0.0017, 0.0020, 0.0022, 0.0025, 0.0028, 0.0030, 0.0032, 0.0034, 0.0036, 0.0038, 0.0039, 0.0040, 0.0041, 0.0041, 0.0041, 0.0041, 0.0040, 0.0039, 0.0038, 0.0036, 0.0034, 0.0032, 0.0030, 0.0028, 0.0025, 0.0022, 0.0020, 0.0017, 0.0015, 0.0012, 0.0010, 0.0008, 0.0006, 0.0004, 0.0003, 0.0002, 0.0001, 0.0000, 0.0000, },
	{0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, }
};
constexpr const float *kHann50x50Raw = &kHann50x50[0][0];

}  // namespace Mosse
