namespace Mosse {
constexpr const unsigned kHann40x40Height = 40;  // Number of rows
constexpr const unsigned kHann40x40Width = 40;  // Number of columns
constexpr const float kHann40x40[kHann40x40Height][kHann40x40Width] = {
	{0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, },
	{0.0000, 0.0000, 0.0002, 0.0004, 0.0006, 0.0010, 0.0014, 0.0018, 0.0023, 0.0028, 0.0034, 0.0039, 0.0044, 0.0049, 0.0053, 0.0057, 0.0060, 0.0062, 0.0064, 0.0065, 0.0065, 0.0064, 0.0062, 0.0060, 0.0057, 0.0053, 0.0049, 0.0044, 0.0039, 0.0034, 0.0028, 0.0023, 0.0018, 0.0014, 0.0010, 0.0006, 0.0004, 0.0002, 0.0000, 0.0000, },
	{0.0000, 0.0002, 0.0007, 0.0015, 0.0026, 0.0040, 0.0056, 0.0074, 0.0093, 0.0113, 0.0134, 0.0154, 0.0174, 0.0193, 0.0210, 0.0225, 0.0237, 0.0247, 0.0254, 0.0257, 0.0257, 0.0254, 0.0247, 0.0237, 0.0225, 0.0210, 0.0193, 0.0174, 0.0154, 0.0134, 0.0113, 0.0093, 0.0074, 0.0056, 0.0040, 0.0026, 0.0015, 0.0007, 0.0002, 0.0000, },
	{0.0000, 0.0004, 0.0015, 0.0033, 0.0057, 0.0088, 0.0124, 0.0164, 0.0207, 0.0252, 0.0298, 0.0344, 0.0388, 0.0430, 0.0467, 0.0501, 0.0528, 0.0550, 0.0564, 0.0572, 0.0572, 0.0564, 0.0550, 0.0528, 0.0501, 0.0467, 0.0430, 0.0388, 0.0344, 0.0298, 0.0252, 0.0207, 0.0164, 0.0124, 0.0088, 0.0057, 0.0033, 0.0015, 0.0004, 0.0000, },
	{0.0000, 0.0006, 0.0026, 0.0057, 0.0101, 0.0154, 0.0217, 0.0286, 0.0362, 0.0441, 0.0522, 0.0602, 0.0679, 0.0752, 0.0818, 0.0877, 0.0925, 0.0963, 0.0988, 0.1001, 0.1001, 0.0988, 0.0963, 0.0925, 0.0877, 0.0818, 0.0752, 0.0679, 0.0602, 0.0522, 0.0441, 0.0362, 0.0286, 0.0217, 0.0154, 0.0101, 0.0057, 0.0026, 0.0006, 0.0000, },
	{0.0000, 0.0010, 0.0040, 0.0088, 0.0154, 0.0236, 0.0332, 0.0439, 0.0554, 0.0676, 0.0799, 0.0922, 0.1041, 0.1152, 0.1254, 0.1343, 0.1417, 0.1475, 0.1514, 0.1534, 0.1534, 0.1514, 0.1475, 0.1417, 0.1343, 0.1254, 0.1152, 0.1041, 0.0922, 0.0799, 0.0676, 0.0554, 0.0439, 0.0332, 0.0236, 0.0154, 0.0088, 0.0040, 0.0010, 0.0000, },
	{0.0000, 0.0014, 0.0056, 0.0124, 0.0217, 0.0332, 0.0466, 0.0617, 0.0779, 0.0950, 0.1123, 0.1296, 0.1463, 0.1620, 0.1763, 0.1888, 0.1993, 0.2073, 0.2128, 0.2156, 0.2156, 0.2128, 0.2073, 0.1993, 0.1888, 0.1763, 0.1620, 0.1463, 0.1296, 0.1123, 0.0950, 0.0779, 0.0617, 0.0466, 0.0332, 0.0217, 0.0124, 0.0056, 0.0014, 0.0000, },
	{0.0000, 0.0018, 0.0074, 0.0164, 0.0286, 0.0439, 0.0617, 0.0816, 0.1031, 0.1256, 0.1486, 0.1714, 0.1935, 0.2142, 0.2332, 0.2497, 0.2635, 0.2742, 0.2815, 0.2852, 0.2852, 0.2815, 0.2742, 0.2635, 0.2497, 0.2332, 0.2142, 0.1935, 0.1714, 0.1486, 0.1256, 0.1031, 0.0816, 0.0617, 0.0439, 0.0286, 0.0164, 0.0074, 0.0018, 0.0000, },
	{0.0000, 0.0023, 0.0093, 0.0207, 0.0362, 0.0554, 0.0779, 0.1031, 0.1302, 0.1587, 0.1877, 0.2165, 0.2444, 0.2707, 0.2946, 0.3155, 0.3330, 0.3465, 0.3556, 0.3603, 0.3603, 0.3556, 0.3465, 0.3330, 0.3155, 0.2946, 0.2707, 0.2444, 0.2165, 0.1877, 0.1587, 0.1302, 0.1031, 0.0779, 0.0554, 0.0362, 0.0207, 0.0093, 0.0023, 0.0000, },
	{0.0000, 0.0028, 0.0113, 0.0252, 0.0441, 0.0676, 0.0950, 0.1256, 0.1587, 0.1934, 0.2287, 0.2638, 0.2978, 0.3298, 0.3589, 0.3844, 0.4057, 0.4221, 0.4333, 0.4390, 0.4390, 0.4333, 0.4221, 0.4057, 0.3844, 0.3589, 0.3298, 0.2978, 0.2638, 0.2287, 0.1934, 0.1587, 0.1256, 0.0950, 0.0676, 0.0441, 0.0252, 0.0113, 0.0028, 0.0000, },
	{0.0000, 0.0034, 0.0134, 0.0298, 0.0522, 0.0799, 0.1123, 0.1486, 0.1877, 0.2287, 0.2705, 0.3121, 0.3523, 0.3901, 0.4245, 0.4547, 0.4799, 0.4993, 0.5126, 0.5193, 0.5193, 0.5126, 0.4993, 0.4799, 0.4547, 0.4245, 0.3901, 0.3523, 0.3121, 0.2705, 0.2287, 0.1877, 0.1486, 0.1123, 0.0799, 0.0522, 0.0298, 0.0134, 0.0034, 0.0000, },
	{0.0000, 0.0039, 0.0154, 0.0344, 0.0602, 0.0922, 0.1296, 0.1714, 0.2165, 0.2638, 0.3121, 0.3600, 0.4064, 0.4500, 0.4897, 0.5246, 0.5536, 0.5760, 0.5913, 0.5990, 0.5990, 0.5913, 0.5760, 0.5536, 0.5246, 0.4897, 0.4500, 0.4064, 0.3600, 0.3121, 0.2638, 0.2165, 0.1714, 0.1296, 0.0922, 0.0602, 0.0344, 0.0154, 0.0039, 0.0000, },
	{0.0000, 0.0044, 0.0174, 0.0388, 0.0679, 0.1041, 0.1463, 0.1935, 0.2444, 0.2978, 0.3523, 0.4064, 0.4587, 0.5080, 0.5528, 0.5921, 0.6249, 0.6502, 0.6675, 0.6762, 0.6762, 0.6675, 0.6502, 0.6249, 0.5921, 0.5528, 0.5080, 0.4587, 0.4064, 0.3523, 0.2978, 0.2444, 0.1935, 0.1463, 0.1041, 0.0679, 0.0388, 0.0174, 0.0044, 0.0000, },
	{0.0000, 0.0049, 0.0193, 0.0430, 0.0752, 0.1152, 0.1620, 0.2142, 0.2707, 0.3298, 0.3901, 0.4500, 0.5080, 0.5625, 0.6122, 0.6557, 0.6919, 0.7200, 0.7391, 0.7488, 0.7488, 0.7391, 0.7200, 0.6919, 0.6557, 0.6122, 0.5625, 0.5080, 0.4500, 0.3901, 0.3298, 0.2707, 0.2142, 0.1620, 0.1152, 0.0752, 0.0430, 0.0193, 0.0049, 0.0000, },
	{0.0000, 0.0053, 0.0210, 0.0467, 0.0818, 0.1254, 0.1763, 0.2332, 0.2946, 0.3589, 0.4245, 0.4897, 0.5528, 0.6122, 0.6662, 0.7136, 0.7530, 0.7836, 0.8044, 0.8149, 0.8149, 0.8044, 0.7836, 0.7530, 0.7136, 0.6662, 0.6122, 0.5528, 0.4897, 0.4245, 0.3589, 0.2946, 0.2332, 0.1763, 0.1254, 0.0818, 0.0467, 0.0210, 0.0053, 0.0000, },
	{0.0000, 0.0057, 0.0225, 0.0501, 0.0877, 0.1343, 0.1888, 0.2497, 0.3155, 0.3844, 0.4547, 0.5246, 0.5921, 0.6557, 0.7136, 0.7643, 0.8066, 0.8393, 0.8616, 0.8728, 0.8728, 0.8616, 0.8393, 0.8066, 0.7643, 0.7136, 0.6557, 0.5921, 0.5246, 0.4547, 0.3844, 0.3155, 0.2497, 0.1888, 0.1343, 0.0877, 0.0501, 0.0225, 0.0057, 0.0000, },
	{0.0000, 0.0060, 0.0237, 0.0528, 0.0925, 0.1417, 0.1993, 0.2635, 0.3330, 0.4057, 0.4799, 0.5536, 0.6249, 0.6919, 0.7530, 0.8066, 0.8512, 0.8857, 0.9092, 0.9211, 0.9211, 0.9092, 0.8857, 0.8512, 0.8066, 0.7530, 0.6919, 0.6249, 0.5536, 0.4799, 0.4057, 0.3330, 0.2635, 0.1993, 0.1417, 0.0925, 0.0528, 0.0237, 0.0060, 0.0000, },
	{0.0000, 0.0062, 0.0247, 0.0550, 0.0963, 0.1475, 0.2073, 0.2742, 0.3465, 0.4221, 0.4993, 0.5760, 0.6502, 0.7200, 0.7836, 0.8393, 0.8857, 0.9216, 0.9460, 0.9584, 0.9584, 0.9460, 0.9216, 0.8857, 0.8393, 0.7836, 0.7200, 0.6502, 0.5760, 0.4993, 0.4221, 0.3465, 0.2742, 0.2073, 0.1475, 0.0963, 0.0550, 0.0247, 0.0062, 0.0000, },
	{0.0000, 0.0064, 0.0254, 0.0564, 0.0988, 0.1514, 0.2128, 0.2815, 0.3556, 0.4333, 0.5126, 0.5913, 0.6675, 0.7391, 0.8044, 0.8616, 0.9092, 0.9460, 0.9712, 0.9839, 0.9839, 0.9712, 0.9460, 0.9092, 0.8616, 0.8044, 0.7391, 0.6675, 0.5913, 0.5126, 0.4333, 0.3556, 0.2815, 0.2128, 0.1514, 0.0988, 0.0564, 0.0254, 0.0064, 0.0000, },
	{0.0000, 0.0065, 0.0257, 0.0572, 0.1001, 0.1534, 0.2156, 0.2852, 0.3603, 0.4390, 0.5193, 0.5990, 0.6762, 0.7488, 0.8149, 0.8728, 0.9211, 0.9584, 0.9839, 0.9968, 0.9968, 0.9839, 0.9584, 0.9211, 0.8728, 0.8149, 0.7488, 0.6762, 0.5990, 0.5193, 0.4390, 0.3603, 0.2852, 0.2156, 0.1534, 0.1001, 0.0572, 0.0257, 0.0065, 0.0000, },
	{0.0000, 0.0065, 0.0257, 0.0572, 0.1001, 0.1534, 0.2156, 0.2852, 0.3603, 0.4390, 0.5193, 0.5990, 0.6762, 0.7488, 0.8149, 0.8728, 0.9211, 0.9584, 0.9839, 0.9968, 0.9968, 0.9839, 0.9584, 0.9211, 0.8728, 0.8149, 0.7488, 0.6762, 0.5990, 0.5193, 0.4390, 0.3603, 0.2852, 0.2156, 0.1534, 0.1001, 0.0572, 0.0257, 0.0065, 0.0000, },
	{0.0000, 0.0064, 0.0254, 0.0564, 0.0988, 0.1514, 0.2128, 0.2815, 0.3556, 0.4333, 0.5126, 0.5913, 0.6675, 0.7391, 0.8044, 0.8616, 0.9092, 0.9460, 0.9712, 0.9839, 0.9839, 0.9712, 0.9460, 0.9092, 0.8616, 0.8044, 0.7391, 0.6675, 0.5913, 0.5126, 0.4333, 0.3556, 0.2815, 0.2128, 0.1514, 0.0988, 0.0564, 0.0254, 0.0064, 0.0000, },
	{0.0000, 0.0062, 0.0247, 0.0550, 0.0963, 0.1475, 0.2073, 0.2742, 0.3465, 0.4221, 0.4993, 0.5760, 0.6502, 0.7200, 0.7836, 0.8393, 0.8857, 0.9216, 0.9460, 0.9584, 0.9584, 0.9460, 0.9216, 0.8857, 0.8393, 0.7836, 0.7200, 0.6502, 0.5760, 0.4993, 0.4221, 0.3465, 0.2742, 0.2073, 0.1475, 0.0963, 0.0550, 0.0247, 0.0062, 0.0000, },
	{0.0000, 0.0060, 0.0237, 0.0528, 0.0925, 0.1417, 0.1993, 0.2635, 0.3330, 0.4057, 0.4799, 0.5536, 0.6249, 0.6919, 0.7530, 0.8066, 0.8512, 0.8857, 0.9092, 0.9211, 0.9211, 0.9092, 0.8857, 0.8512, 0.8066, 0.7530, 0.6919, 0.6249, 0.5536, 0.4799, 0.4057, 0.3330, 0.2635, 0.1993, 0.1417, 0.0925, 0.0528, 0.0237, 0.0060, 0.0000, },
	{0.0000, 0.0057, 0.0225, 0.0501, 0.0877, 0.1343, 0.1888, 0.2497, 0.3155, 0.3844, 0.4547, 0.5246, 0.5921, 0.6557, 0.7136, 0.7643, 0.8066, 0.8393, 0.8616, 0.8728, 0.8728, 0.8616, 0.8393, 0.8066, 0.7643, 0.7136, 0.6557, 0.5921, 0.5246, 0.4547, 0.3844, 0.3155, 0.2497, 0.1888, 0.1343, 0.0877, 0.0501, 0.0225, 0.0057, 0.0000, },
	{0.0000, 0.0053, 0.0210, 0.0467, 0.0818, 0.1254, 0.1763, 0.2332, 0.2946, 0.3589, 0.4245, 0.4897, 0.5528, 0.6122, 0.6662, 0.7136, 0.7530, 0.7836, 0.8044, 0.8149, 0.8149, 0.8044, 0.7836, 0.7530, 0.7136, 0.6662, 0.6122, 0.5528, 0.4897, 0.4245, 0.3589, 0.2946, 0.2332, 0.1763, 0.1254, 0.0818, 0.0467, 0.0210, 0.0053, 0.0000, },
	{0.0000, 0.0049, 0.0193, 0.0430, 0.0752, 0.1152, 0.1620, 0.2142, 0.2707, 0.3298, 0.3901, 0.4500, 0.5080, 0.5625, 0.6122, 0.6557, 0.6919, 0.7200, 0.7391, 0.7488, 0.7488, 0.7391, 0.7200, 0.6919, 0.6557, 0.6122, 0.5625, 0.5080, 0.4500, 0.3901, 0.3298, 0.2707, 0.2142, 0.1620, 0.1152, 0.0752, 0.0430, 0.0193, 0.0049, 0.0000, },
	{0.0000, 0.0044, 0.0174, 0.0388, 0.0679, 0.1041, 0.1463, 0.1935, 0.2444, 0.2978, 0.3523, 0.4064, 0.4587, 0.5080, 0.5528, 0.5921, 0.6249, 0.6502, 0.6675, 0.6762, 0.6762, 0.6675, 0.6502, 0.6249, 0.5921, 0.5528, 0.5080, 0.4587, 0.4064, 0.3523, 0.2978, 0.2444, 0.1935, 0.1463, 0.1041, 0.0679, 0.0388, 0.0174, 0.0044, 0.0000, },
	{0.0000, 0.0039, 0.0154, 0.0344, 0.0602, 0.0922, 0.1296, 0.1714, 0.2165, 0.2638, 0.3121, 0.3600, 0.4064, 0.4500, 0.4897, 0.5246, 0.5536, 0.5760, 0.5913, 0.5990, 0.5990, 0.5913, 0.5760, 0.5536, 0.5246, 0.4897, 0.4500, 0.4064, 0.3600, 0.3121, 0.2638, 0.2165, 0.1714, 0.1296, 0.0922, 0.0602, 0.0344, 0.0154, 0.0039, 0.0000, },
	{0.0000, 0.0034, 0.0134, 0.0298, 0.0522, 0.0799, 0.1123, 0.1486, 0.1877, 0.2287, 0.2705, 0.3121, 0.3523, 0.3901, 0.4245, 0.4547, 0.4799, 0.4993, 0.5126, 0.5193, 0.5193, 0.5126, 0.4993, 0.4799, 0.4547, 0.4245, 0.3901, 0.3523, 0.3121, 0.2705, 0.2287, 0.1877, 0.1486, 0.1123, 0.0799, 0.0522, 0.0298, 0.0134, 0.0034, 0.0000, },
	{0.0000, 0.0028, 0.0113, 0.0252, 0.0441, 0.0676, 0.0950, 0.1256, 0.1587, 0.1934, 0.2287, 0.2638, 0.2978, 0.3298, 0.3589, 0.3844, 0.4057, 0.4221, 0.4333, 0.4390, 0.4390, 0.4333, 0.4221, 0.4057, 0.3844, 0.3589, 0.3298, 0.2978, 0.2638, 0.2287, 0.1934, 0.1587, 0.1256, 0.0950, 0.0676, 0.0441, 0.0252, 0.0113, 0.0028, 0.0000, },
	{0.0000, 0.0023, 0.0093, 0.0207, 0.0362, 0.0554, 0.0779, 0.1031, 0.1302, 0.1587, 0.1877, 0.2165, 0.2444, 0.2707, 0.2946, 0.3155, 0.3330, 0.3465, 0.3556, 0.3603, 0.3603, 0.3556, 0.3465, 0.3330, 0.3155, 0.2946, 0.2707, 0.2444, 0.2165, 0.1877, 0.1587, 0.1302, 0.1031, 0.0779, 0.0554, 0.0362, 0.0207, 0.0093, 0.0023, 0.0000, },
	{0.0000, 0.0018, 0.0074, 0.0164, 0.0286, 0.0439, 0.0617, 0.0816, 0.1031, 0.1256, 0.1486, 0.1714, 0.1935, 0.2142, 0.2332, 0.2497, 0.2635, 0.2742, 0.2815, 0.2852, 0.2852, 0.2815, 0.2742, 0.2635, 0.2497, 0.2332, 0.2142, 0.1935, 0.1714, 0.1486, 0.1256, 0.1031, 0.0816, 0.0617, 0.0439, 0.0286, 0.0164, 0.0074, 0.0018, 0.0000, },
	{0.0000, 0.0014, 0.0056, 0.0124, 0.0217, 0.0332, 0.0466, 0.0617, 0.0779, 0.0950, 0.1123, 0.1296, 0.1463, 0.1620, 0.1763, 0.1888, 0.1993, 0.2073, 0.2128, 0.2156, 0.2156, 0.2128, 0.2073, 0.1993, 0.1888, 0.1763, 0.1620, 0.1463, 0.1296, 0.1123, 0.0950, 0.0779, 0.0617, 0.0466, 0.0332, 0.0217, 0.0124, 0.0056, 0.0014, 0.0000, },
	{0.0000, 0.0010, 0.0040, 0.0088, 0.0154, 0.0236, 0.0332, 0.0439, 0.0554, 0.0676, 0.0799, 0.0922, 0.1041, 0.1152, 0.1254, 0.1343, 0.1417, 0.1475, 0.1514, 0.1534, 0.1534, 0.1514, 0.1475, 0.1417, 0.1343, 0.1254, 0.1152, 0.1041, 0.0922, 0.0799, 0.0676, 0.0554, 0.0439, 0.0332, 0.0236, 0.0154, 0.0088, 0.0040, 0.0010, 0.0000, },
	{0.0000, 0.0006, 0.0026, 0.0057, 0.0101, 0.0154, 0.0217, 0.0286, 0.0362, 0.0441, 0.0522, 0.0602, 0.0679, 0.0752, 0.0818, 0.0877, 0.0925, 0.0963, 0.0988, 0.1001, 0.1001, 0.0988, 0.0963, 0.0925, 0.0877, 0.0818, 0.0752, 0.0679, 0.0602, 0.0522, 0.0441, 0.0362, 0.0286, 0.0217, 0.0154, 0.0101, 0.0057, 0.0026, 0.0006, 0.0000, },
	{0.0000, 0.0004, 0.0015, 0.0033, 0.0057, 0.0088, 0.0124, 0.0164, 0.0207, 0.0252, 0.0298, 0.0344, 0.0388, 0.0430, 0.0467, 0.0501, 0.0528, 0.0550, 0.0564, 0.0572, 0.0572, 0.0564, 0.0550, 0.0528, 0.0501, 0.0467, 0.0430, 0.0388, 0.0344, 0.0298, 0.0252, 0.0207, 0.0164, 0.0124, 0.0088, 0.0057, 0.0033, 0.0015, 0.0004, 0.0000, },
	{0.0000, 0.0002, 0.0007, 0.0015, 0.0026, 0.0040, 0.0056, 0.0074, 0.0093, 0.0113, 0.0134, 0.0154, 0.0174, 0.0193, 0.0210, 0.0225, 0.0237, 0.0247, 0.0254, 0.0257, 0.0257, 0.0254, 0.0247, 0.0237, 0.0225, 0.0210, 0.0193, 0.0174, 0.0154, 0.0134, 0.0113, 0.0093, 0.0074, 0.0056, 0.0040, 0.0026, 0.0015, 0.0007, 0.0002, 0.0000, },
	{0.0000, 0.0000, 0.0002, 0.0004, 0.0006, 0.0010, 0.0014, 0.0018, 0.0023, 0.0028, 0.0034, 0.0039, 0.0044, 0.0049, 0.0053, 0.0057, 0.0060, 0.0062, 0.0064, 0.0065, 0.0065, 0.0064, 0.0062, 0.0060, 0.0057, 0.0053, 0.0049, 0.0044, 0.0039, 0.0034, 0.0028, 0.0023, 0.0018, 0.0014, 0.0010, 0.0006, 0.0004, 0.0002, 0.0000, 0.0000, },
	{0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, }
};
constexpr const float *kHann40x40Raw = &kHann40x40[0][0];

}  // namespace Mosse
