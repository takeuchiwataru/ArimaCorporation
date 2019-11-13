xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 50;
 18.82391;-0.93080;11.41283;,
 15.60113;-0.44620;13.73741;,
 14.75751;3.26981;7.26608;,
 18.93426;1.51716;8.40032;,
 14.81808;4.51240;0.01319;,
 19.03767;2.49019;4.46194;,
 14.87368;5.64958;-2.78010;,
 19.08227;2.96923;2.93803;,
 15.35230;6.46599;0.06646;,
 19.10908;3.08053;4.51785;,
 14.91230;7.23557;3.58009;,
 15.07249;5.35776;8.06582;,
 15.43133;0.44985;14.10553;,
 16.67931;0.59668;13.11239;,
 11.38930;0.12994;12.89175;,
 8.34480;0.47286;12.36628;,
 8.51824;4.06489;4.84368;,
 8.45048;5.26633;-4.32312;,
 8.75733;6.44130;-8.42198;,
 6.07457;8.20212;-5.97376;,
 11.33105;7.57684;-2.71072;,
 8.84480;7.89626;2.85929;,
 8.93307;7.43999;7.25738;,
 8.42148;1.74750;13.08170;,
 10.87709;1.30843;13.84110;,
 4.67896;-0.14446;12.07105;,
 1.16846;3.37927;1.36450;,
 1.07429;-0.15573;10.50311;,
 0.76992;3.78510;-8.16606;,
 0.32192;4.88156;-11.63153;,
 0.36793;5.65238;-7.92249;,
 3.26068;6.74748;2.49675;,
 8.76564;7.55035;-2.85712;,
 -0.01815;6.23700;2.18464;,
 0.63237;1.41897;11.23608;,
 3.95113;1.72537;12.94049;,
 19.88195;0.46097;11.24926;,
 20.47124;1.61494;9.04951;,
 18.95238;-0.31332;11.68042;,
 20.47124;1.61494;9.04951;,
 19.88195;0.46097;11.24926;,
 20.47124;1.61494;9.04951;,
 20.47124;1.61494;9.04951;,
 20.47124;1.61494;9.04951;,
 20.47124;1.61494;9.04951;,
 19.57504;3.25632;9.00715;,
 20.47124;1.61494;9.04951;,
 18.95238;-0.31332;11.68042;,
 20.47124;1.61494;9.04951;,
 16.67931;0.59668;13.11239;;
 
 59;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 3;9,8,10;,
 3;11,12,13;,
 3;1,14,2;,
 3;14,15,2;,
 3;15,16,2;,
 4;2,16,17,4;,
 4;4,17,18,6;,
 3;19,20,18;,
 3;18,20,6;,
 3;20,8,6;,
 3;8,20,21;,
 3;22,23,24;,
 3;15,25,16;,
 3;16,25,26;,
 3;25,27,26;,
 4;16,26,28,17;,
 4;17,28,29,18;,
 4;18,29,30,19;,
 4;19,30,31,32;,
 3;33,34,35;,
 3;36,37,38;,
 3;0,39,40;,
 3;3,41,0;,
 3;5,42,3;,
 3;7,43,5;,
 3;9,44,7;,
 3;45,46,9;,
 3;47,48,45;,
 3;22,35,23;,
 4;22,31,33,35;,
 3;11,24,12;,
 3;11,22,24;,
 3;45,13,47;,
 3;45,11,13;,
 4;9,10,11,45;,
 3;21,22,8;,
 3;8,22,10;,
 3;22,11,10;,
 3;20,19,32;,
 3;20,32,21;,
 4;31,22,21,32;,
 3;30,33,31;,
 4;35,34,27,25;,
 4;15,23,35,25;,
 4;24,23,15,14;,
 3;49,12,38;,
 3;12,1,38;,
 3;1,0,38;,
 3;0,40,38;,
 4;1,12,24,14;,
 3;34,33,27;,
 3;33,30,26;,
 3;26,30,28;,
 3;30,29,28;,
 3;26,27,33;;
 
 MeshMaterialList {
  1;
  59;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  60;
  -0.039462;-0.805893;-0.590745;,
  -0.057548;-0.909341;-0.412053;,
  -0.119591;-0.912557;-0.391073;,
  -0.199545;-0.833612;-0.515046;,
  0.020652;-0.962608;-0.270108;,
  -0.080797;-0.963354;-0.255774;,
  0.113501;-0.942381;-0.314700;,
  0.013625;-0.943423;-0.331311;,
  0.750891;0.657448;-0.062644;,
  0.406724;0.906384;-0.114207;,
  0.384253;0.921994;0.047717;,
  0.288055;0.812730;0.506453;,
  0.459730;0.554872;0.693373;,
  0.445487;0.426701;0.787062;,
  0.129463;-0.945543;-0.298642;,
  0.074440;-0.890259;-0.449330;,
  -0.016009;-0.868979;-0.494590;,
  0.143258;-0.966540;-0.212788;,
  0.214273;-0.929045;-0.301599;,
  0.243712;0.960241;-0.136171;,
  -0.007213;0.993328;-0.115097;,
  0.070605;0.997125;0.027497;,
  -0.007937;0.928904;0.370237;,
  0.064344;0.750468;0.657767;,
  -0.055859;0.716486;0.695362;,
  0.202867;-0.939148;-0.277210;,
  0.157241;-0.921597;-0.354873;,
  0.201146;-0.900759;-0.384934;,
  0.240533;-0.952333;-0.187630;,
  0.294528;-0.912898;-0.282613;,
  -0.155945;0.977992;-0.138611;,
  -0.108469;0.991408;0.073112;,
  -0.220022;0.967375;0.125605;,
  -0.268149;0.919827;0.286383;,
  -0.222099;0.812075;0.539635;,
  -0.314537;0.828369;0.463542;,
  -0.537037;0.797625;0.274564;,
  0.643462;0.628653;0.436750;,
  0.778822;0.616176;-0.117317;,
  0.553137;0.780898;-0.290238;,
  0.217361;0.896988;-0.384923;,
  -0.110051;0.889517;-0.443450;,
  0.700634;-0.532487;0.474942;,
  0.415256;-0.851344;-0.320587;,
  0.777721;-0.556991;0.291396;,
  0.839555;0.514045;0.175800;,
  0.625493;-0.009153;0.780176;,
  -0.187375;-0.479917;0.857071;,
  -0.355240;-0.488988;0.796678;,
  -0.143559;-0.514131;0.845613;,
  -0.236592;-0.551327;0.800039;,
  0.284019;-0.432041;0.855963;,
  -0.202434;-0.540545;0.816598;,
  -0.967993;-0.243668;-0.060134;,
  -0.943359;-0.323795;-0.072314;,
  -0.950169;-0.294684;-0.101681;,
  -0.962081;-0.271587;0.025293;,
  -0.948278;-0.314691;-0.041697;,
  -0.975426;-0.215793;0.044471;,
  -0.974438;-0.217245;0.057236;;
  59;
  4;0,3,2,1;,
  4;1,2,5,4;,
  4;4,5,7,6;,
  4;38,39,9,8;,
  3;8,9,10;,
  3;11,13,12;,
  3;3,16,2;,
  3;16,15,2;,
  3;15,14,2;,
  4;2,14,17,5;,
  4;5,17,18,7;,
  3;20,19,40;,
  3;40,19,39;,
  3;19,9,39;,
  3;9,19,21;,
  3;22,24,23;,
  3;15,27,14;,
  3;14,27,25;,
  3;27,26,25;,
  4;14,25,28,17;,
  4;17,28,29,18;,
  4;40,41,30,20;,
  4;20,30,32,31;,
  3;33,35,34;,
  3;36,36,36;,
  3;42,43,44;,
  3;1,43,0;,
  3;4,43,1;,
  3;6,43,4;,
  3;8,45,38;,
  3;37,45,8;,
  3;46,45,37;,
  3;22,34,24;,
  4;22,32,33,34;,
  3;11,23,13;,
  3;11,22,23;,
  3;37,12,46;,
  3;37,11,12;,
  4;8,10,11,37;,
  3;21,22,9;,
  3;9,22,10;,
  3;22,11,10;,
  3;19,20,31;,
  3;19,31,21;,
  4;32,22,21,31;,
  3;30,33,32;,
  4;47,48,48,47;,
  4;49,49,47,47;,
  4;50,49,49,50;,
  3;12,13,46;,
  3;13,51,46;,
  3;51,42,46;,
  3;42,44,46;,
  4;51,52,50,50;,
  3;53,54,55;,
  3;54,56,57;,
  3;57,56,58;,
  3;56,59,58;,
  3;57,55,54;;
 }
 MeshTextureCoords {
  50;
  0.668563;0.125000;,
  0.668563;0.250000;,
  0.704737;0.250000;,
  0.704737;0.125000;,
  0.740910;0.250000;,
  0.740910;0.125000;,
  0.777084;0.250000;,
  0.777084;0.125000;,
  0.813257;0.250000;,
  0.813257;0.125000;,
  0.840150;0.250000;,
  0.849431;0.250000;,
  0.885604;0.250000;,
  0.885604;0.194411;,
  0.668563;0.313468;,
  0.668563;0.375000;,
  0.704737;0.375000;,
  0.740910;0.375000;,
  0.777084;0.375000;,
  0.813257;0.375000;,
  0.813257;0.328427;,
  0.841071;0.375000;,
  0.849431;0.375000;,
  0.885604;0.375000;,
  0.885604;0.324618;,
  0.668563;0.436855;,
  0.704737;0.500000;,
  0.668563;0.500000;,
  0.740910;0.500000;,
  0.777084;0.500000;,
  0.813257;0.500000;,
  0.849431;0.474905;,
  0.818662;0.375000;,
  0.849431;0.500000;,
  0.885604;0.500000;,
  0.885604;0.443237;,
  0.624656;0.071553;,
  0.614303;0.000000;,
  0.596216;0.125000;,
  0.650477;0.000000;,
  0.640123;0.071553;,
  0.686650;0.000000;,
  0.722824;0.000000;,
  0.758997;0.000000;,
  0.795170;0.000000;,
  0.849431;0.125000;,
  0.831344;0.000000;,
  0.885604;0.125000;,
  0.867517;0.000000;,
  0.756911;0.194411;;
 }
}
