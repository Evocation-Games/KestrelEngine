// Copyright (c) 2023 Tom Hancocks
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <cmath>
#include <libMath/trig/fast_trig.hpp>

// MARK: - Sin

#pragma region Trigonometric Table: Sin
static double trig_table_sin[] = {
    0.0, 0.01745240643728351, 0.03489949670250097, 0.05233595624294383, 0.0697564737441253, 0.08715574274765817,
    0.10452846326765346, 0.12186934340514748, 0.13917310096006544, 0.15643446504023087, 0.17364817766693033,
    0.1908089953765448, 0.20791169081775931, 0.224951054343865, 0.24192189559966773, 0.25881904510252074,
    0.27563735581699916, 0.29237170472273677, 0.3090169943749474, 0.32556815445715664, 0.3420201433256687,
    0.35836794954530027, 0.374606593415912, 0.3907311284892737, 0.40673664307580015, 0.42261826174069944,
    0.4383711467890774, 0.45399049973954675, 0.4694715627858908, 0.48480962024633706, 0.49999999999999994,
    0.5150380749100542, 0.5299192642332049, 0.5446390350150271, 0.5591929034707469, 0.573576436351046,
    0.5877852522924731, 0.6018150231520483, 0.6156614753256582, 0.6293203910498374, 0.6427876096865393,
    0.6560590289905072, 0.6691306063588582, 0.6819983600624985, 0.6946583704589973, 0.7071067811865475,
    0.7193398003386511, 0.7313537016191705, 0.7431448254773941, 0.7547095802227719, 0.766044443118978,
    0.7771459614569708, 0.7880107536067219, 0.7986355100472928, 0.8090169943749473, 0.8191520442889917,
    0.8290375725550416, 0.8386705679454239, 0.848048096156426, 0.8571673007021121, 0.8660254037844386,
    0.8746197071393957, 0.8829475928589269, 0.8910065241883678, 0.898794046299167, 0.9063077870366499,
    0.9135454576426009, 0.9205048534524403, 0.9271838545667874, 0.9335804264972017, 0.9396926207859083,
    0.9455185755993167, 0.9510565162951535, 0.9563047559630354, 0.9612616959383189, 0.9659258262890683,
    0.9702957262759965, 0.9743700647852352, 0.9781476007338056, 0.981627183447664, 0.984807753012208,
    0.9876883405951378, 0.9902680687415703, 0.992546151641322, 0.9945218953682733, 0.9961946980917455,
    0.9975640502598242, 0.9986295347545738, 0.9993908270190958, 0.9998476951563913, 1.0, 0.9998476951563913,
    0.9993908270190958, 0.9986295347545738, 0.9975640502598242, 0.9961946980917455, 0.9945218953682734,
    0.9925461516413221, 0.9902680687415704, 0.9876883405951377, 0.984807753012208, 0.981627183447664,
    0.9781476007338057, 0.9743700647852352, 0.9702957262759965, 0.9659258262890683, 0.9612616959383189,
    0.9563047559630355, 0.9510565162951536, 0.9455185755993168, 0.9396926207859084, 0.9335804264972017,
    0.9271838545667874, 0.9205048534524404, 0.913545457642601, 0.90630778703665, 0.8987940462991669,
    0.8910065241883679, 0.8829475928589271, 0.8746197071393959, 0.8660254037844388, 0.8571673007021123,
    0.8480480961564261, 0.838670567945424, 0.8290375725550418, 0.819152044288992, 0.8090169943749475,
    0.7986355100472928, 0.788010753606722, 0.7771459614569711, 0.7660444431189781, 0.7547095802227719,
    0.7431448254773942, 0.7313537016191707, 0.7193398003386514, 0.7071067811865476, 0.6946583704589971,
    0.6819983600624985, 0.6691306063588583, 0.6560590289905073, 0.6427876096865395, 0.6293203910498377,
    0.6156614753256584, 0.6018150231520482, 0.5877852522924732, 0.5735764363510464, 0.5591929034707469,
    0.5446390350150269, 0.5299192642332049, 0.5150380749100544, 0.49999999999999994, 0.48480962024633717,
    0.4694715627858911, 0.45399049973954686, 0.4383711467890773, 0.4226182617406995, 0.40673664307580043,
    0.39073112848927416, 0.37460659341591224, 0.3583679495453002, 0.3420201433256689, 0.325568154457157,
    0.3090169943749475, 0.292371704722737, 0.27563735581699966, 0.258819045102521, 0.24192189559966773,
    0.22495105434386475, 0.2079116908177593, 0.19080899537654497, 0.17364817766693025, 0.15643446504023098,
    0.13917310096006574, 0.12186934340514755, 0.10452846326765373, 0.08715574274765864, 0.06975647374412552,
    0.05233595624294381, 0.0348994967025007, 0.017452406437283435, 1.2246467991473532e-16, -0.017452406437283192,
    -0.03489949670250089, -0.05233595624294356, -0.06975647374412483, -0.08715574274765794, -0.10452846326765305,
    -0.12186934340514774, -0.13917310096006552, -0.15643446504023076, -0.17364817766693047, -0.19080899537654475,
    -0.20791169081775907, -0.22495105434386498, -0.2419218955996675, -0.25881904510252035, -0.275637355816999,
    -0.2923717047227364, -0.30901699437494773, -0.3255681544571568, -0.34202014332566866, -0.35836794954530043,
    -0.374606593415912, -0.39073112848927355, -0.4067366430757998, -0.4226182617406993, -0.43837114678907707,
    -0.4539904997395463, -0.46947156278589086, -0.48480962024633695, -0.5000000000000001, -0.5150380749100542,
    -0.5299192642332048, -0.5446390350150271, -0.5591929034707467, -0.5735764363510458, -0.587785252292473,
    -0.601815023152048, -0.6156614753256578, -0.6293203910498376, -0.6427876096865393, -0.6560590289905075,
    -0.6691306063588582, -0.6819983600624984, -0.6946583704589974, -0.7071067811865475, -0.7193398003386509,
    -0.7313537016191701, -0.743144825477394, -0.7547095802227717, -0.7660444431189779, -0.7771459614569711,
    -0.788010753606722, -0.7986355100472928, -0.8090169943749473, -0.8191520442889916, -0.8290375725550414,
    -0.838670567945424, -0.8480480961564258, -0.8571673007021121, -0.8660254037844384, -0.8746197071393959,
    -0.882947592858927, -0.8910065241883678, -0.8987940462991668, -0.9063077870366497, -0.913545457642601,
    -0.9205048534524403, -0.9271838545667873, -0.9335804264972016, -0.9396926207859082, -0.9455185755993168,
    -0.9510565162951535, -0.9563047559630353, -0.961261695938319, -0.9659258262890683, -0.9702957262759965,
    -0.9743700647852351, -0.9781476007338056, -0.9816271834476639, -0.984807753012208, -0.9876883405951377,
    -0.9902680687415704, -0.9925461516413221, -0.9945218953682734, -0.9961946980917455, -0.9975640502598242,
    -0.9986295347545738, -0.9993908270190957, -0.9998476951563913, -1.0, -0.9998476951563913, -0.9993908270190958,
    -0.9986295347545738, -0.9975640502598243, -0.9961946980917455, -0.9945218953682734, -0.992546151641322,
    -0.9902680687415704, -0.9876883405951378, -0.9848077530122081, -0.9816271834476641, -0.9781476007338058,
    -0.9743700647852352, -0.9702957262759966, -0.9659258262890682, -0.9612616959383188, -0.9563047559630354,
    -0.9510565162951536, -0.945518575599317, -0.9396926207859085, -0.9335804264972021, -0.9271838545667874,
    -0.9205048534524405, -0.9135454576426008, -0.9063077870366498, -0.898794046299167, -0.891006524188368,
    -0.8829475928589271, -0.8746197071393961, -0.8660254037844386, -0.8571673007021123, -0.8480480961564262,
    -0.8386705679454243, -0.8290375725550421, -0.8191520442889918, -0.8090169943749476, -0.7986355100472932,
    -0.7880107536067218, -0.7771459614569708, -0.7660444431189781, -0.7547095802227722, -0.7431448254773947,
    -0.731353701619171, -0.7193398003386517, -0.7071067811865477, -0.6946583704589976, -0.6819983600624983,
    -0.6691306063588581, -0.6560590289905074, -0.6427876096865396, -0.6293203910498378, -0.6156614753256588,
    -0.6018150231520483, -0.5877852522924732, -0.5735764363510464, -0.5591929034707473, -0.544639035015027,
    -0.5299192642332058, -0.5150380749100545, -0.5000000000000004, -0.48480962024633684, -0.4694715627858908,
    -0.45399049973954697, -0.43837114678907696, -0.4226182617407, -0.40673664307580015, -0.39073112848927466,
    -0.3746065934159123, -0.35836794954530077, -0.34202014332566855, -0.32556815445715753, -0.3090169943749476,
    -0.29237170472273627, -0.2756373558169998, -0.2588190451025207, -0.24192189559966787, -0.22495105434386534,
    -0.20791169081775987, -0.19080899537654467, -0.17364817766693127, -0.15643446504023112, -0.13917310096006588,
    -0.12186934340514811, -0.10452846326765342, -0.08715574274765832, -0.06975647374412476, -0.05233595624294437,
    -0.03489949670250082, -0.01745240643728445,
};
#pragma endregion

auto math::fast::sin(float r) -> float
{
    auto i = abs(static_cast<int>(::floor(r + 0.5f))) % 360;
    return static_cast<float>(trig_table_sin[i]);
}

// MARK: - Cos

#pragma region Trigonometric Table: Cos
static double trig_table_cos[] = {
    1.0, 0.9998476951563913, 0.9993908270190958, 0.9986295347545738, 0.9975640502598242, 0.9961946980917455,
    0.9945218953682733, 0.992546151641322, 0.9902680687415704, 0.9876883405951378, 0.984807753012208,
    0.981627183447664, 0.9781476007338057, 0.9743700647852352, 0.9702957262759965, 0.9659258262890683,
    0.9612616959383189, 0.9563047559630354, 0.9510565162951535, 0.9455185755993168, 0.9396926207859084,
    0.9335804264972017, 0.9271838545667874, 0.9205048534524404, 0.9135454576426009, 0.9063077870366499,
    0.898794046299167, 0.8910065241883679, 0.882947592858927, 0.8746197071393957, 0.8660254037844387,
    0.8571673007021123, 0.848048096156426, 0.838670567945424, 0.8290375725550416, 0.8191520442889918,
    0.8090169943749475, 0.7986355100472928, 0.788010753606722, 0.7771459614569709, 0.7660444431189781,
    0.754709580222772, 0.7431448254773942, 0.7313537016191706, 0.7193398003386512, 0.7071067811865476,
    0.6946583704589974, 0.6819983600624985, 0.6691306063588583, 0.6560590289905074, 0.6427876096865394,
    0.6293203910498376, 0.6156614753256583, 0.6018150231520484, 0.5877852522924732, 0.5735764363510462,
    0.5591929034707469, 0.5446390350150272, 0.5299192642332049, 0.5150380749100544, 0.5000000000000001,
    0.48480962024633717, 0.46947156278589086, 0.4539904997395468, 0.4383711467890774, 0.42261826174069944,
    0.40673664307580015, 0.3907311284892739, 0.37460659341591196, 0.3583679495453004, 0.3420201433256688,
    0.32556815445715676, 0.30901699437494745, 0.29237170472273677, 0.27563735581699916, 0.25881904510252074,
    0.2419218955996679, 0.22495105434386492, 0.20791169081775945, 0.19080899537654492, 0.17364817766693041,
    0.15643446504023092, 0.1391731009600657, 0.12186934340514748, 0.10452846326765344, 0.08715574274765812,
    0.06975647374412546, 0.052335956242943966, 0.03489949670250108, 0.017452406437283376, 6.123233995736766e-17,
    -0.017452406437283473, -0.03489949670250073, -0.05233595624294362, -0.06975647374412534, -0.08715574274765824,
    -0.10452846326765333, -0.12186934340514738, -0.13917310096006535, -0.15643446504023104, -0.1736481776669303,
    -0.1908089953765448, -0.20791169081775912, -0.2249510543438648, -0.24192189559966779, -0.25881904510252085,
    -0.27563735581699905, -0.29237170472273666, -0.30901699437494734, -0.3255681544571564, -0.3420201433256687,
    -0.35836794954530027, -0.37460659341591207, -0.3907311284892736, -0.4067366430758001, -0.42261826174069933,
    -0.4383711467890775, -0.45399049973954675, -0.46947156278589053, -0.48480962024633695, -0.4999999999999998,
    -0.5150380749100542, -0.5299192642332048, -0.5446390350150271, -0.5591929034707467, -0.5735764363510458,
    -0.587785252292473, -0.6018150231520484, -0.6156614753256582, -0.6293203910498373, -0.6427876096865393,
    -0.6560590289905075, -0.6691306063588582, -0.6819983600624984, -0.694658370458997, -0.7071067811865475,
    -0.7193398003386513, -0.7313537016191705, -0.743144825477394, -0.754709580222772, -0.7660444431189779,
    -0.7771459614569707, -0.7880107536067219, -0.7986355100472929, -0.8090169943749473, -0.8191520442889917,
    -0.8290375725550416, -0.8386705679454242, -0.848048096156426, -0.8571673007021121, -0.8660254037844387,
    -0.8746197071393957, -0.8829475928589268, -0.8910065241883678, -0.898794046299167, -0.9063077870366499,
    -0.9135454576426008, -0.9205048534524402, -0.9271838545667873, -0.9335804264972017, -0.9396926207859083,
    -0.9455185755993167, -0.9510565162951535, -0.9563047559630354, -0.9612616959383187, -0.9659258262890682,
    -0.9702957262759965, -0.9743700647852352, -0.9781476007338057, -0.981627183447664, -0.9848077530122081,
    -0.9876883405951377, -0.9902680687415703, -0.992546151641322, -0.9945218953682733, -0.9961946980917455,
    -0.9975640502598242, -0.9986295347545738, -0.9993908270190958, -0.9998476951563913, -1.0, -0.9998476951563913,
    -0.9993908270190958, -0.9986295347545738, -0.9975640502598243, -0.9961946980917455, -0.9945218953682734,
    -0.992546151641322, -0.9902680687415703, -0.9876883405951378, -0.984807753012208, -0.981627183447664,
    -0.9781476007338057, -0.9743700647852352, -0.9702957262759965, -0.9659258262890684, -0.9612616959383189,
    -0.9563047559630355, -0.9510565162951534, -0.9455185755993167, -0.9396926207859084, -0.9335804264972016,
    -0.9271838545667874, -0.9205048534524404, -0.9135454576426011, -0.90630778703665, -0.8987940462991671,
    -0.8910065241883681, -0.8829475928589269, -0.8746197071393959, -0.8660254037844386, -0.8571673007021123,
    -0.8480480961564261, -0.838670567945424, -0.8290375725550418, -0.819152044288992, -0.8090169943749475,
    -0.798635510047293, -0.7880107536067222, -0.7771459614569708, -0.7660444431189781, -0.7547095802227719,
    -0.7431448254773942, -0.7313537016191707, -0.7193398003386511, -0.7071067811865477, -0.6946583704589976,
    -0.6819983600624989, -0.6691306063588585, -0.6560590289905077, -0.6427876096865396, -0.6293203910498372,
    -0.6156614753256582, -0.6018150231520483, -0.5877852522924732, -0.5735764363510464, -0.5591929034707473,
    -0.544639035015027, -0.529919264233205, -0.5150380749100545, -0.5000000000000004, -0.48480962024633684,
    -0.46947156278589075, -0.4539904997395469, -0.43837114678907774, -0.42261826174069994, -0.4067366430758001,
    -0.3907311284892738, -0.3746065934159123, -0.3583679495453007, -0.3420201433256693, -0.32556815445715664,
    -0.30901699437494756, -0.29237170472273705, -0.2756373558169989, -0.25881904510252063, -0.2419218955996678,
    -0.22495105434386528, -0.20791169081775981, -0.19080899537654547, -0.17364817766693033, -0.15643446504023106,
    -0.13917310096006494, -0.12186934340514717, -0.10452846326765335, -0.08715574274765825, -0.06975647374412558,
    -0.052335956242944306, -0.03489949670250164, -0.017452406437283498, -1.8369701987210297e-16,
    0.01745240643728313, 0.03489949670250128, 0.052335956242943946, 0.06975647374412522, 0.08715574274765789,
    0.104528463267653, 0.12186934340514768, 0.13917310096006544, 0.15643446504023067, 0.17364817766692997,
    0.19080899537654422, 0.20791169081775857, 0.2249510543438649, 0.24192189559966742, 0.2588190451025212,
    0.2756373558169994, 0.29237170472273677, 0.30901699437494723, 0.3255681544571563, 0.3420201433256682,
    0.35836794954529955, 0.37460659341591196, 0.3907311284892735, 0.4067366430758006, 0.4226182617406996,
    0.4383711467890774, 0.45399049973954664, 0.4694715627858904, 0.48480962024633645, 0.5, 0.515038074910054,
    0.5299192642332047, 0.5446390350150266, 0.5591929034707462, 0.573576436351046, 0.5877852522924729,
    0.6018150231520479, 0.6156614753256585, 0.6293203910498375, 0.6427876096865393, 0.656059028990507,
    0.6691306063588578, 0.6819983600624979, 0.6946583704589966, 0.7071067811865475, 0.7193398003386509,
    0.7313537016191707, 0.7431448254773942, 0.7547095802227719, 0.7660444431189779, 0.7771459614569706,
    0.7880107536067216, 0.7986355100472928, 0.8090169943749473, 0.8191520442889916, 0.8290375725550414,
    0.838670567945424, 0.8480480961564254, 0.8571673007021121, 0.8660254037844384, 0.8746197071393959,
    0.882947592858927, 0.8910065241883678, 0.8987940462991673, 0.9063077870366497, 0.913545457642601,
    0.9205048534524399, 0.9271838545667873, 0.9335804264972015, 0.9396926207859084, 0.9455185755993165,
    0.9510565162951535, 0.9563047559630357, 0.9612616959383187, 0.9659258262890683, 0.9702957262759965,
    0.9743700647852351, 0.9781476007338056, 0.981627183447664, 0.9848077530122079, 0.9876883405951377,
    0.9902680687415703, 0.992546151641322, 0.9945218953682733, 0.9961946980917455, 0.9975640502598243,
    0.9986295347545738, 0.9993908270190958, 0.9998476951563913,
};
#pragma endregion

auto math::fast::cos(float r) -> float
{
    auto i = abs(static_cast<int>(::floor(r + 0.5f))) % 360;
    return static_cast<float>(trig_table_cos[i]);
}

// MARK: - Tan

#pragma region Trigonometric Table: Tan
static double trig_table_tan[] = {
    0.0, 0.017455064928217585, 0.03492076949174773, 0.0524077792830412, 0.06992681194351041, 0.087488663525924,
    0.10510423526567646, 0.1227845609029046, 0.14054083470239143, 0.15838444032453627, 0.17632698070846498,
    0.19438030913771848, 0.2125565616700221, 0.23086819112556312, 0.2493280028431807, 0.2679491924311227,
    0.2867453857588079, 0.3057306814586604, 0.3249196962329063, 0.3443276132896652, 0.36397023426620234,
    0.3838640350354158, 0.4040262258351568, 0.4244748162096047, 0.4452286853085361, 0.46630765815499864,
    0.4877325885658614, 0.5095254494944288, 0.5317094316614788, 0.554309051452769, 0.5773502691896256,
    0.6008606190275603, 0.6248693519093275, 0.6494075931975105, 0.6745085168424267, 0.7002075382097097,
    0.7265425280053609, 0.7535540501027942, 0.7812856265067172, 0.809784033195007, 0.8390996311772798,
    0.8692867378162265, 0.9004040442978399, 0.9325150861376615, 0.9656887748070739, 0.9999999999999999,
    1.0355303137905694, 1.0723687100246826, 1.1106125148291925, 1.1503684072210092, 1.1917535925942098,
    1.2348971565350508, 1.2799416321930788, 1.3270448216204098, 1.3763819204711731, 1.4281480067421142,
    1.4825609685127399, 1.5398649638145825, 1.6003345290410504, 1.664279482350517, 1.7320508075688767,
    1.8040477552714234, 1.8807264653463316, 1.9626105055051504, 2.0503038415792965, 2.1445069205095586,
    2.2460367739042164, 2.355852365823752, 2.4750868534162964, 2.605089064693801, 2.747477419454621,
    2.9042108776758218, 3.077683537175253, 3.2708526184841404, 3.487414443840909, 3.7320508075688776,
    4.010780933535842, 4.331475874284157, 4.704630109478451, 5.144554015970307, 5.6712818196177075,
    6.313751514675041, 7.115369722384195, 8.144346427974593, 9.514364454222587, 11.43005230276135,
    14.300666256711896, 19.08113668772816, 28.636253282915515, 57.289961630759876, 1.633123935319537e+16,
    -57.28996163075956, -28.6362532829158, -19.08113668772829, -14.300666256711919, -11.430052302761336,
    -9.514364454222598, -8.144346427974602, -7.115369722384214, -6.313751514675036, -5.671281819617711,
    -5.144554015970311, -4.704630109478459, -4.331475874284159, -4.010780933535844, -3.7320508075688763,
    -3.4874144438409105, -3.2708526184841418, -3.0776835371752544, -2.9042108776758253, -2.7474774194546225,
    -2.6050890646938014, -2.4750868534162955, -2.355852365823754, -2.246036773904217, -2.1445069205095595,
    -2.0503038415792956, -1.9626105055051508, -1.8807264653463334, -1.8040477552714242, -1.7320508075688783,
    -1.6642794823505183, -1.6003345290410511, -1.539864963814583, -1.482560968512741, -1.4281480067421155,
    -1.3763819204711738, -1.3270448216204098, -1.279941632193079, -1.2348971565350522, -1.1917535925942102,
    -1.150368407221009, -1.1106125148291928, -1.072368710024683, -1.0355303137905703, -1.0000000000000002,
    -0.9656887748070736, -0.9325150861376617, -0.9004040442978404, -0.8692867378162267, -0.8390996311772804,
    -0.8097840331950077, -0.7812856265067176, -0.7535540501027939, -0.7265425280053611, -0.7002075382097103,
    -0.6745085168424267, -0.6494075931975102, -0.6248693519093275, -0.6008606190275607, -0.5773502691896256,
    -0.5543090514527691, -0.5317094316614792, -0.5095254494944289, -0.48773258856586127, -0.4663076581549987,
    -0.4452286853085365, -0.42447481620960525, -0.40402622583515707, -0.3838640350354157, -0.36397023426620256,
    -0.3443276132896656, -0.32491969623290645, -0.30573068145866067, -0.2867453857588085, -0.267949192431123,
    -0.2493280028431807, -0.23086819112556287, -0.21255656167002207, -0.19438030913771864, -0.17632698070846486,
    -0.1583844403245364, -0.14054083470239176, -0.12278456090290467, -0.10510423526567673, -0.08748866352592446,
    -0.06992681194351064, -0.05240777928304118, -0.03492076949174746, -0.01745506492821751, -1.2246467991473532e-16,
    0.017455064928217266, 0.034920769491747654, 0.05240777928304093, 0.06992681194350994, 0.08748866352592377,
    0.10510423526567604, 0.12278456090290486, 0.14054083470239154, 0.15838444032453616, 0.17632698070846511,
    0.19438030913771842, 0.21255656167002185, 0.2308681911255631, 0.24932800284318046, 0.26794919243112225,
    0.28674538575880776, 0.30573068145865995, 0.3249196962329067, 0.34432761328966544, 0.3639702342662023,
    0.383864035035416, 0.4040262258351568, 0.4244748162096045, 0.44522868530853565, 0.46630765815499836,
    0.48773258856586094, 0.5095254494944281, 0.5317094316614789, 0.5543090514527688, 0.577350269189626,
    0.6008606190275603, 0.6248693519093272, 0.6494075931975105, 0.6745085168424263, 0.7002075382097093,
    0.7265425280053608, 0.7535540501027936, 0.7812856265067166, 0.8097840331950075, 0.8390996311772798,
    0.869286737816227, 0.9004040442978399, 0.9325150861376613, 0.9656887748070743, 0.9999999999999997,
    1.0355303137905687, 1.0723687100246813, 1.110612514829192, 1.1503684072210083, 1.1917535925942093,
    1.2348971565350524, 1.279941632193079, 1.32704482162041, 1.3763819204711731, 1.4281480067421135,
    1.4825609685127383, 1.5398649638145832, 1.60033452904105, 1.6642794823505167, 1.7320508075688752,
    1.8040477552714247, 1.8807264653463323, 1.96261050550515, 2.0503038415792942, 2.1445069205095555,
    2.246036773904217, 2.3558523658237522, 2.4750868534162938, 2.605089064693798, 2.747477419454617,
    2.904210877675823, 3.077683537175252, 3.270852618484137, 3.487414443840913, 3.7320508075688794,
    4.010780933535843, 4.331475874284149, 4.704630109478443, 5.144554015970292, 5.67128181961771,
    6.313751514675035, 7.115369722384235, 8.144346427974614, 9.514364454222598, 11.430052302761334,
    14.30066625671187, 19.081136687728037, 28.63625328291505, 57.28996163075948, 5443746451065123.0,
    -57.289961630760686, -28.63625328291535, -19.08113668772817, -14.300666256711946, -11.43005230276138,
    -9.514364454222628, -8.14434642797458, -7.11536972238421, -6.313751514675051, -5.671281819617723,
    -5.144554015970327, -4.704630109478472, -4.331475874284157, -4.01078093353585, -3.732050807568871,
    -3.487414443840906, -3.2708526184841404, -3.077683537175256, -2.9042108776758266, -2.747477419454627,
    -2.6050890646938076, -2.4750868534162964, -2.355852365823755, -2.2460367739042137, -2.1445069205095573,
    -2.0503038415792965, -1.9626105055051515, -1.8807264653463338, -1.8040477552714267, -1.7320508075688772,
    -1.6642794823505185, -1.6003345290410516, -1.5398649638145847, -1.4825609685127425, -1.4281480067421146,
    -1.3763819204711742, -1.3270448216204114, -1.279941632193078, -1.234897156535051, -1.1917535925942102,
    -1.1503684072210103, -1.1106125148291943, -1.0723687100246841, -1.0355303137905714, -1.0000000000000002,
    -0.9656887748070748, -0.9325150861376611, -0.9004040442978398, -0.8692867378162269, -0.8390996311772805,
    -0.8097840331950079, -0.7812856265067185, -0.7535540501027942, -0.7265425280053611, -0.7002075382097103,
    -0.6745085168424275, -0.6494075931975104, -0.6248693519093289, -0.6008606190275608, -0.5773502691896265,
    -0.5543090514527687, -0.5317094316614788, -0.509525449494429, -0.48773258856586077, -0.46630765815499936,
    -0.44522868530853604, -0.4244748162096059, -0.4040262258351571, -0.38386403503541644, -0.3639702342662021,
    -0.34432761328966627, -0.32491969623290656, -0.30573068145865984, -0.2867453857588086, -0.26794919243112264,
    -0.24932800284318085, -0.23086819112556348, -0.21255656167002268, -0.19438030913771834, -0.17632698070846595,
    -0.15838444032453655, -0.1405408347023919, -0.12278456090290524, -0.10510423526567642, -0.08748866352592415,
    -0.06992681194350987, -0.052407779283041744, -0.03492076949174758, -0.017455064928218522,
};
#pragma endregion

auto math::fast::tan(float r) -> float
{
    auto i = abs(static_cast<int>(::floor(r + 0.5f))) % 360;
    return static_cast<float>(trig_table_tan[i]);
}