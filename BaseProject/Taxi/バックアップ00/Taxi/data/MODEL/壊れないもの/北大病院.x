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
 106;
 136.10750;0.05356;37.50371;,
 136.10750;0.05356;-38.90756;,
 136.10750;117.92481;-38.90756;,
 136.10750;117.92481;37.50371;,
 136.10750;0.05356;-38.90756;,
 -136.10423;0.05356;-38.90756;,
 -136.10423;117.92481;-38.90756;,
 136.10750;117.92481;-38.90756;,
 136.10750;117.92481;-38.90756;,
 -136.10423;117.92481;-38.90756;,
 -136.10423;117.92481;37.50371;,
 136.10750;117.92481;37.50371;,
 32.34557;117.72897;-39.13543;,
 32.34557;117.72897;-47.55968;,
 -32.43671;117.72897;-39.13543;,
 -0.04556;117.72897;-49.98587;,
 -32.43669;117.72897;-47.55968;,
 32.36664;117.72897;45.81350;,
 32.34207;117.72897;37.38934;,
 -0.01727;117.72897;48.33426;,
 -32.43995;117.72897;37.57837;,
 -32.41537;117.72897;46.00254;,
 32.44506;36.06032;-47.59264;,
 -0.03592;36.03420;-50.04503;,
 -0.04556;117.72897;-49.98587;,
 32.34557;117.72897;-47.55968;,
 -32.43496;36.06705;-47.57617;,
 -32.43669;117.72897;-47.55968;,
 32.44506;36.06032;-39.22945;,
 32.44506;36.06032;-47.59264;,
 32.34557;117.72897;-47.55968;,
 32.34557;117.72897;-39.13543;,
 -32.43496;36.06705;-47.57617;,
 -32.43324;36.06032;-39.22945;,
 -32.43671;117.72897;-39.13543;,
 -32.43669;117.72897;-47.55968;,
 -32.42365;36.00577;46.02188;,
 -0.02950;35.99878;48.36695;,
 -0.01727;117.72897;48.33426;,
 -32.41537;117.72897;46.00254;,
 32.36420;36.00619;45.79817;,
 32.36664;117.72897;45.81350;,
 -32.50725;35.94547;37.63353;,
 -32.42365;36.00577;46.02188;,
 -32.41537;117.72897;46.00254;,
 -32.43995;117.72897;37.57837;,
 32.36420;36.00619;45.79817;,
 32.31720;36.01174;37.53816;,
 32.34207;117.72897;37.38934;,
 32.36664;117.72897;45.81350;,
 32.44506;36.06032;-39.22945;,
 -0.41749;35.99451;-39.32340;,
 -0.03592;36.03420;-50.04503;,
 32.44506;36.06032;-47.59264;,
 -32.43324;36.06032;-39.22945;,
 -32.43496;36.06705;-47.57617;,
 136.10750;117.92481;37.50371;,
 -136.10423;117.92481;37.50371;,
 -136.10423;0.05356;37.50371;,
 136.10750;0.05356;37.50371;,
 -136.10423;0.05356;-38.90756;,
 -136.10423;0.05356;37.50371;,
 -136.10423;117.92481;37.50371;,
 -136.10423;117.92481;-38.90756;,
 -0.01727;36.00495;48.33426;,
 -32.50725;35.94547;37.63353;,
 0.22154;35.95340;37.84361;,
 -0.02950;35.99878;48.36695;,
 -32.42365;36.00577;46.02188;,
 32.31720;36.01174;37.53816;,
 32.36420;36.00619;45.79817;,
 29.11686;118.01435;-39.27412;,
 -29.35289;118.01435;-39.27412;,
 -29.35289;141.44069;-39.27412;,
 29.11686;141.44069;-39.27412;,
 29.11686;141.44069;-39.27412;,
 -29.35289;141.44069;-39.27412;,
 -29.35289;141.44069;-21.25725;,
 29.11686;141.44069;-21.25725;,
 29.11686;141.44069;-21.25725;,
 -29.35289;141.44069;-21.25725;,
 -29.35289;118.01435;-21.25725;,
 29.11686;118.01435;-21.25725;,
 -29.35289;118.01435;-39.27412;,
 -29.35289;118.01435;-21.25725;,
 -29.35289;141.44069;-21.25725;,
 -29.35289;141.44069;-39.27412;,
 29.11686;118.01435;-21.25725;,
 29.11686;118.01435;-39.27412;,
 29.11686;141.44069;-39.27412;,
 29.11686;141.44069;-21.25725;,
 29.11686;141.44069;-39.27412;,
 29.11686;141.44069;-21.25725;,
 -29.35289;141.44069;-21.25725;,
 -29.35289;141.44069;-39.27412;,
 29.11686;141.44069;-21.25725;,
 29.11686;118.01435;-21.25725;,
 -29.35289;118.01435;-21.25725;,
 -29.35289;141.44069;-21.25725;,
 -29.35289;118.01435;-39.27412;,
 -29.35289;141.44069;-39.27412;,
 -29.35289;141.44069;-21.25725;,
 29.11686;118.01435;-21.25725;,
 29.11686;141.44069;-21.25725;,
 29.11686;141.44069;-39.27412;,
 29.11686;118.01435;-39.27412;;
 
 41;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 3;12,13,14;,
 3;13,15,14;,
 3;15,16,14;,
 3;17,18,19;,
 3;18,20,19;,
 3;20,21,19;,
 4;22,23,24,25;,
 4;23,26,27,24;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;37,40,41,38;,
 4;42,43,44,45;,
 4;46,47,48,49;,
 4;50,51,52,53;,
 4;51,54,55,52;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;56,57,58,59;,
 4;60,61,62,63;,
 3;17,18,19;,
 3;18,20,19;,
 3;20,21,19;,
 4;36,64,38,39;,
 4;64,40,41,38;,
 4;42,43,44,45;,
 4;46,47,48,49;,
 4;65,66,67,68;,
 4;66,69,70,67;,
 4;71,72,73,74;,
 4;75,76,77,78;,
 4;79,80,81,82;,
 4;83,84,85,86;,
 4;87,88,89,90;,
 4;91,92,93,94;,
 4;95,96,97,98;,
 4;99,100,101,81;,
 4;102,103,104,105;;
 
 MeshMaterialList {
  1;
  41;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\game107\\Desktop\\�k��a�@.jpg";
   }
  }
 }
 MeshNormals {
  25;
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.074991;0.000613;-0.997184;,
  -0.000174;0.000536;-1.000000;,
  -0.075337;0.000456;-0.997158;,
  0.999999;0.001218;0.000000;,
  -1.000000;-0.000032;0.000102;,
  -0.071737;0.000224;0.997424;,
  0.003176;0.000213;0.999995;,
  0.078085;0.000002;0.996947;,
  -0.999979;0.000466;0.006439;,
  0.999991;-0.000172;-0.004303;,
  -0.000069;-0.999998;-0.002055;,
  0.000000;0.000000;1.000000;,
  0.003190;0.000014;0.999995;,
  0.000513;-0.999993;0.003782;,
  0.003183;0.000113;0.999995;,
  0.001542;-0.999997;-0.001832;,
  -0.001679;-0.999996;-0.002278;,
  -1.000000;0.000000;0.000000;,
  -0.000166;-0.999984;0.005743;,
  0.001192;-0.999998;0.001822;,
  0.000000;-1.000000;-0.000000;;
  41;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,3,2;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;9,10,18,9;,
  4;10,11,11,18;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;19,14,14,19;,
  4;14,20,20,14;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;15,15,15,15;,
  4;21,21,21,21;,
  3;2,2,2;,
  3;2,2,2;,
  3;2,2,2;,
  4;9,16,18,9;,
  4;16,11,11,18;,
  4;12,12,12,12;,
  4;13,13,13,13;,
  4;22,17,17,22;,
  4;17,23,23,17;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;15,15,15,15;,
  4;21,21,21,21;,
  4;0,0,0,0;,
  4;24,24,24,24;,
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;21,21,21,21;;
 }
 MeshTextureCoords {
  106;
  0.275525;0.997673;,
  -0.000085;0.997673;,
  -0.000085;0.498805;,
  0.275525;0.498805;,
  0.000228;0.489995;,
  0.496674;0.489995;,
  0.496674;0.002858;,
  0.000228;0.002858;,
  0.327276;0.923112;,
  0.397157;0.923112;,
  0.397157;0.759488;,
  0.327276;0.759488;,
  0.326810;0.761139;,
  0.326810;0.924763;,
  0.403402;0.761139;,
  0.451810;0.924763;,
  0.403402;0.924763;,
  0.403402;0.920916;,
  0.403402;0.757292;,
  0.451810;0.920916;,
  0.326810;0.757292;,
  0.326810;0.920916;,
  0.787792;0.512769;,
  0.878477;0.512769;,
  0.878477;0.001293;,
  0.787792;0.001293;,
  0.969868;0.512769;,
  0.969868;0.001293;,
  0.788532;0.509995;,
  0.967619;0.509995;,
  0.967619;0.002011;,
  0.788532;0.002011;,
  0.787161;0.510025;,
  0.966256;0.510025;,
  0.966256;0.002040;,
  0.787161;0.002040;,
  0.785268;0.502144;,
  0.876738;0.502144;,
  0.876738;0.004079;,
  0.785268;0.004079;,
  0.968915;0.502144;,
  0.968915;0.004079;,
  0.788553;0.508168;,
  0.967180;0.508168;,
  0.967180;0.000183;,
  0.788553;0.000183;,
  0.789359;0.508594;,
  0.966309;0.508594;,
  0.966309;0.000609;,
  0.789359;0.000609;,
  0.321055;0.917268;,
  0.378525;0.917268;,
  0.378525;0.753644;,
  0.321055;0.753644;,
  0.435994;0.917268;,
  0.435994;0.753644;,
  -0.002326;-0.000547;,
  0.492569;-0.000547;,
  0.492569;0.503184;,
  -0.002326;0.503184;,
  0.994062;0.496447;,
  0.750922;0.496447;,
  0.750922;0.002061;,
  0.994062;0.002061;,
  0.876738;0.502144;,
  0.336319;0.919388;,
  0.389862;0.919388;,
  0.389862;0.755764;,
  0.336319;0.755764;,
  0.443406;0.919388;,
  0.443406;0.755764;,
  0.287588;0.734353;,
  0.460520;0.734353;,
  0.460520;0.562158;,
  0.287588;0.562158;,
  0.283793;0.953919;,
  0.446993;0.953919;,
  0.446993;0.892096;,
  0.283793;0.892096;,
  0.283793;0.954258;,
  0.446993;0.954258;,
  0.446993;0.892435;,
  0.283793;0.892435;,
  0.284900;0.955096;,
  0.448099;0.955096;,
  0.448099;0.893273;,
  0.284900;0.893273;,
  0.286690;0.953580;,
  0.449890;0.953580;,
  0.449890;0.891756;,
  0.286690;0.891756;,
  0.274514;0.978568;,
  0.274514;0.866770;,
  0.455950;0.866770;,
  0.455950;0.978568;,
  0.455950;0.978568;,
  0.455950;0.866770;,
  0.446993;0.892096;,
  0.446993;0.953919;,
  0.455950;0.867383;,
  0.274514;0.867383;,
  0.283793;0.892435;,
  0.275743;0.868898;,
  0.275743;0.980696;,
  0.284900;0.955096;,
  0.284900;0.893273;;
 }
}
