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
 7.52956;-0.56864;4.56512;,
 6.24044;-0.37480;5.49496;,
 5.90300;1.11160;2.90644;,
 7.57372;0.41056;3.36012;,
 5.92724;1.60864;0.00528;,
 7.61508;0.79976;1.78476;,
 5.94948;2.06352;-1.11204;,
 7.63292;0.99136;1.17520;,
 6.14092;2.39008;0.02660;,
 7.64364;1.03588;1.80712;,
 5.96492;2.69792;1.43204;,
 6.02900;1.94680;3.22632;,
 6.17252;-0.01640;5.64220;,
 6.67172;0.04236;5.24496;,
 4.55572;-0.14436;5.15672;,
 3.33792;-0.00716;4.94652;,
 3.40728;1.42964;1.93748;,
 3.38020;1.91020;-1.72924;,
 3.50292;2.38020;-3.36880;,
 2.42984;3.08452;-2.38952;,
 4.53244;2.83440;-1.08428;,
 3.53792;2.96220;1.14372;,
 3.57324;2.77968;2.90296;,
 3.36860;0.50268;5.23268;,
 4.35084;0.32704;5.53644;,
 1.87160;-0.25412;4.82840;,
 0.46740;1.15540;0.54580;,
 0.42972;-0.25860;4.20124;,
 0.30796;1.31772;-3.26644;,
 0.12876;1.75632;-4.65260;,
 0.14716;2.06464;-3.16900;,
 1.30428;2.50268;0.99868;,
 3.50624;2.82380;-1.14284;,
 -0.00728;2.29848;0.87384;,
 0.25296;0.37128;4.49444;,
 1.58044;0.49384;5.17620;,
 7.95280;-0.01192;4.49972;,
 8.18848;0.44964;3.61980;,
 7.58096;-0.32164;4.67216;,
 8.18848;0.44964;3.61980;,
 7.95280;-0.01192;4.49972;,
 8.18848;0.44964;3.61980;,
 8.18848;0.44964;3.61980;,
 8.18848;0.44964;3.61980;,
 8.18848;0.44964;3.61980;,
 7.83000;1.10620;3.60288;,
 8.18848;0.44964;3.61980;,
 7.58096;-0.32164;4.67216;,
 8.18848;0.44964;3.61980;,
 6.67172;0.04236;5.24496;;
 
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
  -0.039471;-0.805887;-0.590752;,
  -0.057564;-0.909339;-0.412054;,
  -0.119593;-0.912556;-0.391075;,
  -0.199543;-0.833610;-0.515051;,
  0.020620;-0.962612;-0.270099;,
  -0.080797;-0.963355;-0.255772;,
  0.113458;-0.942390;-0.314688;,
  0.013622;-0.943424;-0.331310;,
  0.750896;0.657443;-0.062639;,
  0.406721;0.906385;-0.114208;,
  0.384254;0.921994;0.047714;,
  0.288049;0.812728;0.506459;,
  0.459720;0.554855;0.693393;,
  0.445473;0.426678;0.787083;,
  0.129468;-0.945543;-0.298641;,
  0.074442;-0.890258;-0.449331;,
  -0.016016;-0.868976;-0.494595;,
  0.143258;-0.966539;-0.212791;,
  0.214277;-0.929041;-0.301608;,
  0.243708;0.960241;-0.136172;,
  -0.007210;0.993328;-0.115094;,
  0.070605;0.997125;0.027491;,
  -0.007936;0.928903;0.370237;,
  0.064348;0.750464;0.657772;,
  -0.055851;0.716484;0.695364;,
  0.202860;-0.939149;-0.277212;,
  0.157219;-0.921600;-0.354873;,
  0.201150;-0.900757;-0.384935;,
  0.240530;-0.952333;-0.187635;,
  0.294527;-0.912895;-0.282625;,
  -0.155943;0.977992;-0.138610;,
  -0.108469;0.991408;0.073109;,
  -0.220022;0.967375;0.125604;,
  -0.268153;0.919826;0.286381;,
  -0.222098;0.812076;0.539634;,
  -0.314541;0.828369;0.463539;,
  -0.537051;0.797622;0.274546;,
  0.643469;0.628648;0.436749;,
  0.778828;0.616170;-0.117313;,
  0.553131;0.780904;-0.290232;,
  0.217360;0.896990;-0.384920;,
  -0.110049;0.889518;-0.443448;,
  0.700620;-0.532502;0.474945;,
  0.415220;-0.851360;-0.320590;,
  0.777705;-0.557009;0.291402;,
  0.839563;0.514031;0.175799;,
  0.625486;-0.009176;0.780181;,
  -0.187376;-0.479927;0.857065;,
  -0.355239;-0.488998;0.796672;,
  -0.143568;-0.514130;0.845612;,
  -0.236595;-0.551317;0.800045;,
  0.284019;-0.432039;0.855964;,
  -0.202424;-0.540534;0.816607;,
  -0.967996;-0.243659;-0.060119;,
  -0.943354;-0.323812;-0.072314;,
  -0.950166;-0.294694;-0.101681;,
  -0.962078;-0.271599;0.025295;,
  -0.948271;-0.314711;-0.041699;,
  -0.975425;-0.215795;0.044473;,
  -0.974437;-0.217247;0.057233;;
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
  0.668560;0.125000;,
  0.668560;0.250000;,
  0.704740;0.250000;,
  0.704740;0.125000;,
  0.740910;0.250000;,
  0.740910;0.125000;,
  0.777080;0.250000;,
  0.777080;0.125000;,
  0.813260;0.250000;,
  0.813260;0.125000;,
  0.840150;0.250000;,
  0.849430;0.250000;,
  0.885600;0.250000;,
  0.885600;0.194410;,
  0.668560;0.313470;,
  0.668560;0.375000;,
  0.704740;0.375000;,
  0.740910;0.375000;,
  0.777080;0.375000;,
  0.813260;0.375000;,
  0.813260;0.328430;,
  0.841070;0.375000;,
  0.849430;0.375000;,
  0.885600;0.375000;,
  0.885600;0.324620;,
  0.668560;0.436850;,
  0.704740;0.500000;,
  0.668560;0.500000;,
  0.740910;0.500000;,
  0.777080;0.500000;,
  0.813260;0.500000;,
  0.849430;0.474910;,
  0.818660;0.375000;,
  0.849430;0.500000;,
  0.885600;0.500000;,
  0.885600;0.443240;,
  0.624660;0.071550;,
  0.614300;0.000000;,
  0.596220;0.125000;,
  0.650480;0.000000;,
  0.640120;0.071550;,
  0.686650;0.000000;,
  0.722820;0.000000;,
  0.759000;0.000000;,
  0.795170;0.000000;,
  0.849430;0.125000;,
  0.831340;0.000000;,
  0.885600;0.125000;,
  0.867520;0.000000;,
  0.756910;0.194410;;
 }
}