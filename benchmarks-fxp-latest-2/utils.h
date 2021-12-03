#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
//#include "structs.h"

float lookup[1201] = {
    0.002473, 0.002497, 0.002522, 0.002548, 0.002573, 0.002599, 0.002625,
    0.002651, 0.002678, 0.002705, 0.002732, 0.002759, 0.002787, 0.002815,
    0.002843, 0.002872, 0.002900, 0.002929, 0.002959, 0.002988, 0.003018,
    0.003049, 0.003079, 0.003110, 0.003141, 0.003173, 0.003204, 0.003237,
    0.003269, 0.003302, 0.003335, 0.003368, 0.003402, 0.003436, 0.003470,
    0.003505, 0.003540, 0.003576, 0.003612, 0.003648, 0.003684, 0.003721,
    0.003758, 0.003796, 0.003834, 0.003872, 0.003911, 0.003950, 0.003990,
    0.004030, 0.004070, 0.004111, 0.004152, 0.004194, 0.004236, 0.004278,
    0.004321, 0.004364, 0.004408, 0.004452, 0.004496, 0.004541, 0.004587,
    0.004633, 0.004679, 0.004726, 0.004773, 0.004821, 0.004869, 0.004918,
    0.004967, 0.005016, 0.005067, 0.005117, 0.005168, 0.005220, 0.005272,
    0.005325, 0.005378, 0.005432, 0.005486, 0.005541, 0.005597, 0.005652,
    0.005709, 0.005766, 0.005824, 0.005882, 0.005941, 0.006000, 0.006060,
    0.006120, 0.006181, 0.006243, 0.006306, 0.006369, 0.006432, 0.006496,
    0.006561, 0.006627, 0.006693, 0.006760, 0.006827, 0.006895, 0.006964,
    0.007034, 0.007104, 0.007175, 0.007246, 0.007319, 0.007392, 0.007465,
    0.007540, 0.007615, 0.007691, 0.007768, 0.007845, 0.007923, 0.008002,
    0.008082, 0.008163, 0.008244, 0.008326, 0.008409, 0.008493, 0.008577,
    0.008663, 0.008749, 0.008836, 0.008924, 0.009013, 0.009103, 0.009194,
    0.009285, 0.009378, 0.009471, 0.009565, 0.009661, 0.009757, 0.009854,
    0.009952, 0.010051, 0.010151, 0.010252, 0.010354, 0.010457, 0.010561,
    0.010666, 0.010772, 0.010879, 0.010987, 0.011096, 0.011206, 0.011318,
    0.011430, 0.011544, 0.011658, 0.011774, 0.011891, 0.012009, 0.012128,
    0.012249, 0.012370, 0.012493, 0.012617, 0.012742, 0.012869, 0.012996,
    0.013125, 0.013255, 0.013387, 0.013520, 0.013654, 0.013789, 0.013926,
    0.014064, 0.014203, 0.014344, 0.014486, 0.014629, 0.014774, 0.014920,
    0.015068, 0.015217, 0.015368, 0.015520, 0.015673, 0.015828, 0.015985,
    0.016143, 0.016303, 0.016464, 0.016626, 0.016791, 0.016957, 0.017124,
    0.017293, 0.017464, 0.017636, 0.017810, 0.017986, 0.018164, 0.018343,
    0.018524, 0.018707, 0.018891, 0.019077, 0.019265, 0.019455, 0.019647,
    0.019840, 0.020036, 0.020233, 0.020432, 0.020633, 0.020836, 0.021041,
    0.021248, 0.021457, 0.021668, 0.021881, 0.022096, 0.022313, 0.022533,
    0.022754, 0.022977, 0.023203, 0.023431, 0.023661, 0.023893, 0.024127,
    0.024364, 0.024602, 0.024844, 0.025087, 0.025333, 0.025581, 0.025831,
    0.026084, 0.026339, 0.026597, 0.026857, 0.027120, 0.027385, 0.027652,
    0.027923, 0.028195, 0.028471, 0.028749, 0.029029, 0.029312, 0.029598,
    0.029887, 0.030178, 0.030472, 0.030769, 0.031069, 0.031371, 0.031676,
    0.031984, 0.032295, 0.032609, 0.032926, 0.033246, 0.033569, 0.033895,
    0.034224, 0.034556, 0.034891, 0.035230, 0.035571, 0.035916, 0.036264,
    0.036615, 0.036969, 0.037327, 0.037688, 0.038052, 0.038420, 0.038791,
    0.039166, 0.039544, 0.039925, 0.040310, 0.040699, 0.041091, 0.041487,
    0.041887, 0.042290, 0.042697, 0.043107, 0.043522, 0.043940, 0.044362,
    0.044788, 0.045218, 0.045651, 0.046089, 0.046531, 0.046976, 0.047426,
    0.047880, 0.048338, 0.048800, 0.049266, 0.049737, 0.050211, 0.050690,
    0.051174, 0.051661, 0.052154, 0.052650, 0.053151, 0.053657, 0.054167,
    0.054681, 0.055201, 0.055724, 0.056253, 0.056786, 0.057324, 0.057867,
    0.058415, 0.058967, 0.059524, 0.060087, 0.060654, 0.061226, 0.061804,
    0.062386, 0.062973, 0.063566, 0.064164, 0.064767, 0.065375, 0.065989,
    0.066608, 0.067232, 0.067862, 0.068498, 0.069138, 0.069785, 0.070437,
    0.071094, 0.071758, 0.072427, 0.073101, 0.073782, 0.074468, 0.075160,
    0.075858, 0.076562, 0.077272, 0.077988, 0.078710, 0.079439, 0.080173,
    0.080914, 0.081660, 0.082413, 0.083173, 0.083938, 0.084711, 0.085489,
    0.086274, 0.087066, 0.087864, 0.088669, 0.089480, 0.090298, 0.091123,
    0.091955, 0.092793, 0.093638, 0.094490, 0.095350, 0.096216, 0.097089,
    0.097969, 0.098856, 0.099751, 0.100652, 0.101561, 0.102477, 0.103400,
    0.104331, 0.105269, 0.106215, 0.107168, 0.108129, 0.109097, 0.110073,
    0.111056, 0.112047, 0.113046, 0.114052, 0.115067, 0.116089, 0.117119,
    0.118157, 0.119203, 0.120257, 0.121319, 0.122389, 0.123467, 0.124553,
    0.125648, 0.126751, 0.127862, 0.128981, 0.130109, 0.131245, 0.132389,
    0.133542, 0.134703, 0.135873, 0.137051, 0.138238, 0.139434, 0.140638,
    0.141851, 0.143073, 0.144303, 0.145542, 0.146790, 0.148047, 0.149313,
    0.150588, 0.151871, 0.153164, 0.154465, 0.155776, 0.157096, 0.158424,
    0.159762, 0.161109, 0.162465, 0.163830, 0.165205, 0.166589, 0.167982,
    0.169384, 0.170796, 0.172216, 0.173647, 0.175086, 0.176535, 0.177994,
    0.179462, 0.180939, 0.182426, 0.183922, 0.185427, 0.186943, 0.188467,
    0.190002, 0.191545, 0.193099, 0.194662, 0.196234, 0.197816, 0.199408,
    0.201009, 0.202620, 0.204240, 0.205870, 0.207510, 0.209159, 0.210818,
    0.212487, 0.214165, 0.215853, 0.217550, 0.219257, 0.220974, 0.222700,
    0.224436, 0.226181, 0.227937, 0.229701, 0.231475, 0.233259, 0.235052,
    0.236855, 0.238667, 0.240489, 0.242320, 0.244161, 0.246011, 0.247871,
    0.249740, 0.251618, 0.253506, 0.255403, 0.257310, 0.259225, 0.261150,
    0.263084, 0.265027, 0.266980, 0.268941, 0.270912, 0.272892, 0.274881,
    0.276878, 0.278885, 0.280900, 0.282925, 0.284958, 0.287000, 0.289051,
    0.291110, 0.293178, 0.295254, 0.297339, 0.299433, 0.301535, 0.303645,
    0.305764, 0.307891, 0.310026, 0.312169, 0.314320, 0.316479, 0.318646,
    0.320821, 0.323004, 0.325195, 0.327393, 0.329599, 0.331812, 0.334033,
    0.336261, 0.338497, 0.340740, 0.342990, 0.345247, 0.347511, 0.349782,
    0.352059, 0.354344, 0.356635, 0.358933, 0.361237, 0.363548, 0.365864,
    0.368188, 0.370517, 0.372852, 0.375194, 0.377541, 0.379894, 0.382252,
    0.384616, 0.386986, 0.389361, 0.391741, 0.394126, 0.396517, 0.398912,
    0.401312, 0.403717, 0.406127, 0.408541, 0.410960, 0.413382, 0.415809,
    0.418241, 0.420676, 0.423115, 0.425557, 0.428004, 0.430454, 0.432907,
    0.435364, 0.437824, 0.440286, 0.442752, 0.445221, 0.447692, 0.450166,
    0.452642, 0.455121, 0.457602, 0.460085, 0.462570, 0.465057, 0.467546,
    0.470036, 0.472528, 0.475021, 0.477515, 0.480011, 0.482507, 0.485005,
    0.487503, 0.490001, 0.492501, 0.495000, 0.497500, 0.500000, 0.502500,
    0.505000, 0.507499, 0.509999, 0.512497, 0.514996, 0.517493, 0.519989,
    0.522485, 0.524979, 0.527472, 0.529964, 0.532454, 0.534943, 0.537430,
    0.539915, 0.542398, 0.544879, 0.547358, 0.549834, 0.552308, 0.554779,
    0.557248, 0.559714, 0.562176, 0.564636, 0.567093, 0.569546, 0.571996,
    0.574443, 0.576885, 0.579324, 0.581759, 0.584191, 0.586618, 0.589040,
    0.591459, 0.593873, 0.596283, 0.598688, 0.601088, 0.603483, 0.605874,
    0.608259, 0.610639, 0.613014, 0.615384, 0.617748, 0.620106, 0.622459,
    0.624806, 0.627148, 0.629483, 0.631812, 0.634136, 0.636452, 0.638763,
    0.641067, 0.643365, 0.645656, 0.647941, 0.650218, 0.652489, 0.654753,
    0.657010, 0.659260, 0.661503, 0.663739, 0.665967, 0.668188, 0.670401,
    0.672607, 0.674805, 0.676996, 0.679179, 0.681354, 0.683521, 0.685680,
    0.687831, 0.689974, 0.692109, 0.694236, 0.696355, 0.698465, 0.700567,
    0.702661, 0.704746, 0.706822, 0.708890, 0.710949, 0.713000, 0.715042,
    0.717075, 0.719100, 0.721115, 0.723122, 0.725119, 0.727108, 0.729088,
    0.731059, 0.733020, 0.734973, 0.736916, 0.738850, 0.740775, 0.742691,
    0.744597, 0.746494, 0.748382, 0.750260, 0.752129, 0.753989, 0.755839,
    0.757680, 0.759511, 0.761333, 0.763145, 0.764948, 0.766741, 0.768525,
    0.770299, 0.772063, 0.773818, 0.775564, 0.777300, 0.779026, 0.780743,
    0.782450, 0.784147, 0.785835, 0.787513, 0.789182, 0.790841, 0.792490,
    0.794130, 0.795760, 0.797380, 0.798991, 0.800592, 0.802184, 0.803766,
    0.805338, 0.806901, 0.808455, 0.809998, 0.811533, 0.813057, 0.814573,
    0.816078, 0.817574, 0.819061, 0.820538, 0.822006, 0.823465, 0.824914,
    0.826353, 0.827784, 0.829204, 0.830616, 0.832018, 0.833411, 0.834795,
    0.836170, 0.837535, 0.838891, 0.840238, 0.841576, 0.842904, 0.844224,
    0.845535, 0.846836, 0.848129, 0.849412, 0.850687, 0.851953, 0.853210,
    0.854458, 0.855697, 0.856927, 0.858149, 0.859362, 0.860566, 0.861762,
    0.862949, 0.864127, 0.865297, 0.866458, 0.867611, 0.868755, 0.869891,
    0.871019, 0.872138, 0.873249, 0.874352, 0.875447, 0.876533, 0.877611,
    0.878681, 0.879743, 0.880797, 0.881843, 0.882881, 0.883911, 0.884933,
    0.885948, 0.886954, 0.887953, 0.888944, 0.889927, 0.890903, 0.891871,
    0.892832, 0.893785, 0.894731, 0.895669, 0.896599, 0.897523, 0.898439,
    0.899348, 0.900249, 0.901144, 0.902031, 0.902911, 0.903784, 0.904651,
    0.905510, 0.906362, 0.907207, 0.908045, 0.908877, 0.909702, 0.910520,
    0.911331, 0.912136, 0.912934, 0.913726, 0.914511, 0.915289, 0.916062,
    0.916827, 0.917587, 0.918340, 0.919086, 0.919827, 0.920561, 0.921290,
    0.922012, 0.922728, 0.923438, 0.924142, 0.924840, 0.925532, 0.926218,
    0.926899, 0.927573, 0.928242, 0.928906, 0.929563, 0.930215, 0.930862,
    0.931502, 0.932138, 0.932768, 0.933392, 0.934011, 0.934625, 0.935233,
    0.935836, 0.936434, 0.937027, 0.937614, 0.938196, 0.938774, 0.939346,
    0.939913, 0.940476, 0.941033, 0.941585, 0.942133, 0.942676, 0.943214,
    0.943747, 0.944276, 0.944799, 0.945319, 0.945833, 0.946343, 0.946849,
    0.947350, 0.947846, 0.948339, 0.948826, 0.949310, 0.949789, 0.950263,
    0.950734, 0.951200, 0.951662, 0.952120, 0.952574, 0.953024, 0.953470,
    0.953911, 0.954349, 0.954782, 0.955212, 0.955638, 0.956060, 0.956478,
    0.956893, 0.957303, 0.957710, 0.958113, 0.958513, 0.958909, 0.959301,
    0.959690, 0.960075, 0.960456, 0.960834, 0.961209, 0.961580, 0.961948,
    0.962312, 0.962673, 0.963031, 0.963385, 0.963736, 0.964084, 0.964429,
    0.964770, 0.965109, 0.965444, 0.965776, 0.966105, 0.966431, 0.966754,
    0.967074, 0.967391, 0.967704, 0.968016, 0.968324, 0.968629, 0.968931,
    0.969231, 0.969528, 0.969822, 0.970113, 0.970402, 0.970688, 0.970971,
    0.971251, 0.971529, 0.971805, 0.972077, 0.972348, 0.972615, 0.972880,
    0.973143, 0.973403, 0.973661, 0.973916, 0.974169, 0.974419, 0.974667,
    0.974913, 0.975156, 0.975398, 0.975636, 0.975873, 0.976107, 0.976339,
    0.976569, 0.976797, 0.977023, 0.977246, 0.977467, 0.977687, 0.977904,
    0.978119, 0.978332, 0.978543, 0.978752, 0.978959, 0.979164, 0.979367,
    0.979568, 0.979767, 0.979964, 0.980160, 0.980353, 0.980545, 0.980735,
    0.980923, 0.981109, 0.981293, 0.981476, 0.981657, 0.981836, 0.982014,
    0.982190, 0.982364, 0.982536, 0.982707, 0.982876, 0.983043, 0.983209,
    0.983374, 0.983536, 0.983697, 0.983857, 0.984015, 0.984172, 0.984327,
    0.984480, 0.984632, 0.984783, 0.984932, 0.985080, 0.985226, 0.985371,
    0.985514, 0.985656, 0.985797, 0.985936, 0.986074, 0.986211, 0.986346,
    0.986480, 0.986613, 0.986744, 0.986875, 0.987004, 0.987131, 0.987258,
    0.987383, 0.987507, 0.987630, 0.987751, 0.987872, 0.987991, 0.988109,
    0.988226, 0.988342, 0.988456, 0.988570, 0.988682, 0.988794, 0.988904,
    0.989013, 0.989121, 0.989228, 0.989334, 0.989439, 0.989543, 0.989646,
    0.989748, 0.989849, 0.989949, 0.990048, 0.990146, 0.990243, 0.990339,
    0.990435, 0.990529, 0.990622, 0.990715, 0.990806, 0.990897, 0.990987,
    0.991076, 0.991164, 0.991251, 0.991337, 0.991422, 0.991507, 0.991591,
    0.991674, 0.991756, 0.991837, 0.991918, 0.991998, 0.992077, 0.992155,
    0.992232, 0.992309, 0.992385, 0.992460, 0.992535, 0.992608, 0.992681,
    0.992754, 0.992825, 0.992896, 0.992966, 0.993036, 0.993105, 0.993173,
    0.993240, 0.993307, 0.993373, 0.993439, 0.993504, 0.993568, 0.993632,
    0.993694, 0.993757, 0.993819, 0.993880, 0.993940, 0.994000, 0.994059,
    0.994118, 0.994176, 0.994234, 0.994291, 0.994348, 0.994403, 0.994459,
    0.994514, 0.994568, 0.994622, 0.994675, 0.994728, 0.994780, 0.994832,
    0.994883, 0.994933, 0.994984, 0.995033, 0.995082, 0.995131, 0.995179,
    0.995227, 0.995274, 0.995321, 0.995367, 0.995413, 0.995459, 0.995504,
    0.995548, 0.995592, 0.995636, 0.995679, 0.995722, 0.995764, 0.995806,
    0.995848, 0.995889, 0.995930, 0.995970, 0.996010, 0.996050, 0.996089,
    0.996128, 0.996166, 0.996204, 0.996242, 0.996279, 0.996316, 0.996352,
    0.996388, 0.996424, 0.996460, 0.996495, 0.996530, 0.996564, 0.996598,
    0.996632, 0.996665, 0.996698, 0.996731, 0.996763, 0.996796, 0.996827,
    0.996859, 0.996890, 0.996921, 0.996951, 0.996982, 0.997012, 0.997041,
    0.997071, 0.997100, 0.997128, 0.997157, 0.997185, 0.997213, 0.997241,
    0.997268, 0.997295, 0.997322, 0.997349, 0.997375, 0.997401, 0.997427,
    0.997452, 0.997478, 0.997503, 0.997527};

float lookuptanh[801] = {
    -0.999329, -0.999316, -0.999302, -0.999288, -0.999273, -0.999259, -0.999244,
    -0.999229, -0.999213, -0.999197, -0.999181, -0.999164, -0.999148, -0.999130,
    -0.999113, -0.999095, -0.999077, -0.999058, -0.999039, -0.999019, -0.999000,
    -0.998979, -0.998959, -0.998938, -0.998916, -0.998894, -0.998872, -0.998849,
    -0.998826, -0.998802, -0.998778, -0.998754, -0.998728, -0.998703, -0.998676,
    -0.998650, -0.998623, -0.998595, -0.998566, -0.998538, -0.998508, -0.998478,
    -0.998447, -0.998416, -0.998384, -0.998351, -0.998318, -0.998284, -0.998249,
    -0.998214, -0.998178, -0.998141, -0.998104, -0.998065, -0.998026, -0.997986,
    -0.997946, -0.997904, -0.997862, -0.997819, -0.997775, -0.997730, -0.997684,
    -0.997638, -0.997590, -0.997541, -0.997492, -0.997441, -0.997389, -0.997337,
    -0.997283, -0.997228, -0.997172, -0.997115, -0.997057, -0.996998, -0.996937,
    -0.996875, -0.996812, -0.996748, -0.996682, -0.996615, -0.996547, -0.996478,
    -0.996407, -0.996334, -0.996260, -0.996185, -0.996108, -0.996029, -0.995949,
    -0.995868, -0.995784, -0.995699, -0.995613, -0.995524, -0.995434, -0.995342,
    -0.995248, -0.995152, -0.995055, -0.994955, -0.994853, -0.994750, -0.994644,
    -0.994536, -0.994426, -0.994314, -0.994199, -0.994082, -0.993963, -0.993842,
    -0.993718, -0.993591, -0.993462, -0.993330, -0.993196, -0.993059, -0.992919,
    -0.992777, -0.992631, -0.992483, -0.992332, -0.992178, -0.992020, -0.991860,
    -0.991696, -0.991529, -0.991358, -0.991185, -0.991007, -0.990827, -0.990642,
    -0.990454, -0.990262, -0.990066, -0.989867, -0.989663, -0.989455, -0.989243,
    -0.989027, -0.988807, -0.988582, -0.988353, -0.988119, -0.987880, -0.987637,
    -0.987389, -0.987136, -0.986878, -0.986614, -0.986346, -0.986072, -0.985792,
    -0.985507, -0.985217, -0.984921, -0.984618, -0.984310, -0.983996, -0.983675,
    -0.983348, -0.983014, -0.982674, -0.982327, -0.981973, -0.981613, -0.981245,
    -0.980869, -0.980487, -0.980096, -0.979698, -0.979292, -0.978879, -0.978457,
    -0.978026, -0.977587, -0.977140, -0.976683, -0.976218, -0.975743, -0.975259,
    -0.974766, -0.974262, -0.973749, -0.973226, -0.972693, -0.972149, -0.971594,
    -0.971029, -0.970452, -0.969864, -0.969265, -0.968653, -0.968030, -0.967395,
    -0.966747, -0.966087, -0.965414, -0.964727, -0.964028, -0.963314, -0.962587,
    -0.961846, -0.961090, -0.960319, -0.959534, -0.958733, -0.957917, -0.957085,
    -0.956237, -0.955373, -0.954492, -0.953594, -0.952679, -0.951746, -0.950795,
    -0.949826, -0.948838, -0.947832, -0.946806, -0.945760, -0.944695, -0.943609,
    -0.942503, -0.941375, -0.940227, -0.939056, -0.937863, -0.936648, -0.935409,
    -0.934147, -0.932861, -0.931552, -0.930217, -0.928858, -0.927472, -0.926062,
    -0.924624, -0.923160, -0.921668, -0.920149, -0.918602, -0.917026, -0.915420,
    -0.913786, -0.912120, -0.910425, -0.908698, -0.906939, -0.905148, -0.903325,
    -0.901468, -0.899577, -0.897653, -0.895693, -0.893698, -0.891667, -0.889599,
    -0.887494, -0.885352, -0.883171, -0.880951, -0.878692, -0.876393, -0.874053,
    -0.871672, -0.869249, -0.866784, -0.864275, -0.861723, -0.859126, -0.856485,
    -0.853797, -0.851064, -0.848283, -0.845455, -0.842579, -0.839654, -0.836679,
    -0.833655, -0.830579, -0.827452, -0.824272, -0.821040, -0.817754, -0.814414,
    -0.811019, -0.807569, -0.804062, -0.800499, -0.796878, -0.793199, -0.789461,
    -0.785664, -0.781806, -0.777888, -0.773908, -0.769866, -0.765762, -0.761594,
    -0.757362, -0.753066, -0.748704, -0.744277, -0.739783, -0.735222, -0.730594,
    -0.725897, -0.721132, -0.716298, -0.711394, -0.706419, -0.701374, -0.696257,
    -0.691069, -0.685809, -0.680476, -0.675070, -0.669590, -0.664037, -0.658409,
    -0.652707, -0.646929, -0.641077, -0.635149, -0.629145, -0.623065, -0.616909,
    -0.610677, -0.604368, -0.597982, -0.591519, -0.584980, -0.578363, -0.571670,
    -0.564899, -0.558052, -0.551128, -0.544127, -0.537049, -0.529895, -0.522665,
    -0.515359, -0.507977, -0.500520, -0.492988, -0.485381, -0.477700, -0.469945,
    -0.462117, -0.454216, -0.446244, -0.438199, -0.430084, -0.421899, -0.413644,
    -0.405321, -0.396930, -0.388473, -0.379949, -0.371360, -0.362707, -0.353992,
    -0.345214, -0.336375, -0.327477, -0.318521, -0.309507, -0.300437, -0.291312,
    -0.282135, -0.272905, -0.263625, -0.254295, -0.244919, -0.235496, -0.226028,
    -0.216518, -0.206966, -0.197375, -0.187746, -0.178081, -0.168381, -0.158649,
    -0.148885, -0.139093, -0.129273, -0.119427, -0.109558, -0.099668, -0.089758,
    -0.079830, -0.069886, -0.059928, -0.049958, -0.039979, -0.029991, -0.019997,
    -0.010000, 0.000000,  0.010000,  0.019997,  0.029991,  0.039979,  0.049959,
    0.059928,  0.069886,  0.079830,  0.089758,  0.099668,  0.109559,  0.119427,
    0.129273,  0.139092,  0.148885,  0.158648,  0.168381,  0.178081,  0.187746,
    0.197375,  0.206966,  0.216518,  0.226028,  0.235495,  0.244919,  0.254296,
    0.263625,  0.272905,  0.282135,  0.291313,  0.300437,  0.309507,  0.318521,
    0.327477,  0.336375,  0.345214,  0.353992,  0.362707,  0.371360,  0.379949,
    0.388472,  0.396930,  0.405321,  0.413644,  0.421899,  0.430084,  0.438199,
    0.446244,  0.454216,  0.462117,  0.469945,  0.477700,  0.485381,  0.492988,
    0.500520,  0.507977,  0.515359,  0.522665,  0.529896,  0.537049,  0.544127,
    0.551128,  0.558052,  0.564899,  0.571670,  0.578363,  0.584980,  0.591519,
    0.597982,  0.604368,  0.610677,  0.616909,  0.623065,  0.629145,  0.635149,
    0.641077,  0.646929,  0.652707,  0.658409,  0.664037,  0.669590,  0.675070,
    0.680476,  0.685809,  0.691069,  0.696258,  0.701374,  0.706419,  0.711393,
    0.716298,  0.721132,  0.725897,  0.730594,  0.735222,  0.739783,  0.744277,
    0.748704,  0.753066,  0.757362,  0.761594,  0.765762,  0.769866,  0.773908,
    0.777888,  0.781806,  0.785664,  0.789461,  0.793199,  0.796878,  0.800499,
    0.804062,  0.807569,  0.811019,  0.814414,  0.817754,  0.821040,  0.824272,
    0.827451,  0.830579,  0.833654,  0.836679,  0.839654,  0.842579,  0.845455,
    0.848283,  0.851064,  0.853797,  0.856485,  0.859126,  0.861723,  0.864275,
    0.866784,  0.869249,  0.871672,  0.874053,  0.876393,  0.878692,  0.880951,
    0.883171,  0.885352,  0.887494,  0.889599,  0.891667,  0.893698,  0.895693,
    0.897653,  0.899577,  0.901468,  0.903325,  0.905148,  0.906939,  0.908698,
    0.910425,  0.912120,  0.913786,  0.915420,  0.917026,  0.918602,  0.920149,
    0.921668,  0.923160,  0.924624,  0.926062,  0.927472,  0.928858,  0.930217,
    0.931552,  0.932861,  0.934147,  0.935409,  0.936648,  0.937863,  0.939056,
    0.940227,  0.941375,  0.942503,  0.943609,  0.944695,  0.945760,  0.946806,
    0.947832,  0.948838,  0.949826,  0.950795,  0.951746,  0.952679,  0.953594,
    0.954492,  0.955373,  0.956237,  0.957085,  0.957917,  0.958733,  0.959534,
    0.960319,  0.961090,  0.961846,  0.962587,  0.963314,  0.964028,  0.964727,
    0.965414,  0.966087,  0.966747,  0.967395,  0.968030,  0.968653,  0.969265,
    0.969864,  0.970452,  0.971029,  0.971594,  0.972149,  0.972693,  0.973226,
    0.973749,  0.974262,  0.974766,  0.975259,  0.975743,  0.976218,  0.976683,
    0.977140,  0.977587,  0.978026,  0.978457,  0.978879,  0.979292,  0.979698,
    0.980096,  0.980487,  0.980869,  0.981245,  0.981613,  0.981973,  0.982327,
    0.982674,  0.983014,  0.983348,  0.983675,  0.983996,  0.984310,  0.984618,
    0.984921,  0.985217,  0.985507,  0.985792,  0.986072,  0.986346,  0.986614,
    0.986878,  0.987136,  0.987389,  0.987637,  0.987880,  0.988119,  0.988353,
    0.988582,  0.988807,  0.989027,  0.989243,  0.989455,  0.989663,  0.989867,
    0.990066,  0.990262,  0.990454,  0.990642,  0.990827,  0.991007,  0.991185,
    0.991358,  0.991529,  0.991696,  0.991860,  0.992020,  0.992178,  0.992332,
    0.992483,  0.992631,  0.992777,  0.992919,  0.993059,  0.993196,  0.993330,
    0.993462,  0.993591,  0.993718,  0.993842,  0.993963,  0.994082,  0.994199,
    0.994314,  0.994426,  0.994536,  0.994644,  0.994750,  0.994853,  0.994955,
    0.995055,  0.995152,  0.995248,  0.995342,  0.995434,  0.995524,  0.995613,
    0.995699,  0.995784,  0.995868,  0.995949,  0.996029,  0.996108,  0.996185,
    0.996260,  0.996334,  0.996407,  0.996478,  0.996547,  0.996615,  0.996682,
    0.996748,  0.996812,  0.996875,  0.996937,  0.996998,  0.997057,  0.997115,
    0.997172,  0.997228,  0.997283,  0.997337,  0.997389,  0.997441,  0.997492,
    0.997541,  0.997590,  0.997638,  0.997684,  0.997730,  0.997775,  0.997819,
    0.997862,  0.997904,  0.997946,  0.997986,  0.998026,  0.998065,  0.998104,
    0.998141,  0.998178,  0.998214,  0.998249,  0.998284,  0.998318,  0.998351,
    0.998384,  0.998416,  0.998447,  0.998478,  0.998508,  0.998538,  0.998566,
    0.998595,  0.998623,  0.998650,  0.998676,  0.998703,  0.998728,  0.998754,
    0.998778,  0.998802,  0.998826,  0.998849,  0.998872,  0.998894,  0.998916,
    0.998938,  0.998959,  0.998979,  0.999000,  0.999019,  0.999039,  0.999058,
    0.999077,  0.999095,  0.999113,  0.999130,  0.999148,  0.999164,  0.999181,
    0.999197,  0.999213,  0.999229,  0.999244,  0.999259,  0.999273,  0.999288,
    0.999302,  0.999316,  0.999329};

float tanhFunctionLUT(float input) {
  int lookTableIndex = (int)(input * 100);
  lookTableIndex = lookTableIndex + 400;
  float value;
  if (lookTableIndex < 0)
    return -1;
  else if (lookTableIndex >= 800)
    return 1;
  else {
    value = lookuptanh[lookTableIndex];
    return value;
  }
}

void activeTanhLUT(float *layer, int sizeLayer) {
  unsigned int i;
  for (i = 0; i < sizeLayer; i++) {
    layer[i] = tanhFunctionLUT(layer[i]);
  }
}

float sigmoidLUT(float input) {
  int lookTableIndex = (int)(input * 100);
  lookTableIndex = lookTableIndex + 600;
  float value;
  if (lookTableIndex < 0)
    return 0;
  else if (lookTableIndex >= 1200)
    return 1;
  else {
    value = lookup[lookTableIndex];
    return value;
  }
}

void activeSigmoidLUT(float *layer, int sizeLayer) {
  unsigned int i;
  for (i = 0; i < sizeLayer; i++) {
    layer[i] = sigmoidLUT(layer[i]);
  }
}

void printfVector(float *pointer, int size) {
  for (int i = 0; i < size; i++) {
    printf("%.6f \n", pointer[i]);
  }
}

void printIVector(int *pointer, int size) {
  for (int i = 0; i < size; i++) {
    printf("%d \n", pointer[i]);
  }
}

// void printfLayers(esbmc_nnet* net) {
//   for(int i = 0; i < net->layersNumber; i++) {
//     printf("Layer: %d Neurons: %d \n", i, net->layers[i].neurons);
//   }
// }

void printfMatrix(float *mat, int rows, int column) {
  int i = 0, j = 0;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < column; j++) {
      printf("  %.6f", mat[i * column + j]);
    }
    printf("\n");
  }
}

void printfLayerResults(float *mat, int neurons, int layer) {
  int i = 0;
  printf("Layer %d\n", layer);
  for (i = 0; i < neurons; i++) {
    printf("Neuron%d  %.6f\n", i, mat[i]);
  }
}

void normalizef(float *image, int size) {
  int i;
  for (i = 0; i < size; i++) {
    image[i] = (image[i]) / 255;
  }
}

float *normalizefVec(float *image, int size) {
  float *out = (float *)malloc(size * sizeof(float));
  int i;
  for (i = 0; i < size; i++) {
    out[i] = (image[i]) / 255;
  }
  return out;
}

int biggest(float *vec, int size) {
  int i = 0;
  float biggest = vec[0];
  int biggestIndex = 0;
  for (i = 1; i < size; i++) {
    if (vec[i] > biggest) {
      biggestIndex = i;
      biggest = vec[i];
    }
  }
  return biggestIndex;
}

void setVectorValue(float *ptr, float value, int size) {
  for (int i = 0; i < size; i++) {
    ptr[i] = value;
  }
}

void RELU(float *activationPotencial, int size) {
  int i;
  for (i = 0; i < size; i++) {
    if (activationPotencial[i] < 0)
      activationPotencial[i] = 0;
  }
}

void transposeMatrix(float *matrix, int rows, int columns) {
  printfMatrix(matrix, rows, columns);
  float *buffer;
  buffer = (float *)malloc(sizeof(float) * rows * columns);
  for (int w = 0; w < rows * columns; w++) {
    buffer[w] = matrix[w];
  }
  for (int j = 0; j < columns; j++) {
    for (int i = 0; i < rows; i++) {
      matrix[i + j * rows] = buffer[i * columns + j];
    }
  }
  free(buffer);
  printfMatrix(matrix, columns, rows);
}

#endif