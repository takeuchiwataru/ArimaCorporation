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
 32;
 0.40124;-0.05296;1.25348;,
 0.20796;-1.71836;0.88552;,
 0.69904;-1.24572;0.19340;,
 1.24272;-0.26052;0.01156;,
 1.05236;-0.95920;-0.27208;,
 0.43060;-1.20968;-0.48504;,
 0.75896;-0.79744;-0.91320;,
 -0.02700;0.05152;-1.64288;,
 0.02940;-0.56972;-0.85132;,
 -0.43268;-1.39872;0.74892;,
 -0.57312;-1.85964;0.50156;,
 -0.04796;-2.03056;0.73916;,
 0.30236;-2.05524;0.34924;,
 -0.46700;0.51484;0.31996;,
 0.11036;-1.94128;-0.24724;,
 0.30236;-2.05524;0.34924;,
 0.08772;-3.09364;-0.00028;,
 -0.04760;-2.90968;-0.59156;,
 -0.28616;-1.59668;-0.45504;,
 -0.60540;-2.77868;-0.73292;,
 -0.57312;-1.85964;0.50156;,
 -0.66048;-1.46000;0.16368;,
 -1.08292;-2.75812;-0.28732;,
 -0.90336;-2.89788;0.28852;,
 -0.04796;-2.03056;0.73916;,
 -0.31380;-3.09320;0.41396;,
 0.11036;-1.94128;-0.24724;,
 -0.28616;-1.59668;-0.45504;,
 -0.66048;-1.46000;0.16368;,
 -0.02700;0.05152;-1.64288;,
 0.02940;-0.56972;-0.85132;,
 -0.46700;0.51484;0.31996;;
 
 26;
 4;0,1,2,3;,
 4;4,2,5,6;,
 3;3,2,4;,
 4;7,6,5,8;,
 4;1,9,10,11;,
 4;2,1,11,12;,
 4;3,4,6,7;,
 4;13,9,1,0;,
 4;14,15,16,17;,
 4;18,14,17,19;,
 4;20,21,22,23;,
 4;15,24,25,16;,
 4;24,20,23,25;,
 4;5,2,12,26;,
 4;8,5,26,27;,
 4;9,13,28,10;,
 4;13,8,27,28;,
 3;29,30,31;,
 3;16,25,17;,
 3;25,23,17;,
 3;17,23,19;,
 3;23,22,19;,
 3;19,22,18;,
 3;22,28,18;,
 4;0,3,29,13;,
 4;21,18,19,22;;
 
 MeshMaterialList {
  1;
  26;
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
  41;
  -0.052881;-0.558667;-0.827704;,
  0.870294;-0.461521;-0.172009;,
  0.095512;-0.834319;-0.542945;,
  -0.652519;-0.065858;0.754905;,
  0.333597;-0.684797;-0.647894;,
  0.772324;-0.398461;0.494716;,
  0.853742;-0.441786;0.275589;,
  0.676867;-0.218855;-0.702818;,
  -0.711132;-0.034180;0.702227;,
  -0.368774;-0.194698;0.908900;,
  0.902228;-0.409881;0.134100;,
  -0.694406;0.025878;0.719118;,
  0.675766;-0.361083;0.642619;,
  0.956093;-0.079250;-0.282144;,
  0.755120;0.051881;-0.653530;,
  0.826509;-0.325025;0.459610;,
  0.812820;-0.260240;0.521151;,
  -0.694430;-0.545565;-0.469176;,
  -0.909241;-0.079796;0.408551;,
  0.406511;0.172869;-0.897143;,
  0.302377;-0.111217;-0.946678;,
  -0.940499;0.265225;-0.212408;,
  -0.879021;0.374768;-0.294738;,
  0.445358;-0.838449;-0.314101;,
  -0.448551;-0.132470;0.883886;,
  0.701119;-0.448192;0.554577;,
  0.859509;0.091648;-0.502837;,
  -0.502026;-0.013130;0.864753;,
  -0.348906;-0.139161;0.926768;,
  -0.858766;0.090496;-0.504313;,
  -0.936388;-0.095468;-0.337733;,
  -0.738533;0.312318;-0.597517;,
  -0.244986;-0.940248;-0.236464;,
  -0.252037;-0.938014;-0.237923;,
  -0.229994;-0.949251;-0.214534;,
  -0.215431;-0.957301;-0.192778;,
  -0.193242;-0.966460;-0.169154;,
  -0.201359;-0.964425;-0.171284;,
  -0.661623;0.358800;-0.658421;,
  -0.635040;0.335138;-0.695994;,
  0.414110;0.910197;0.007398;;
  26;
  4;16,5,6,15;,
  4;1,23,4,2;,
  3;15,6,1;,
  4;17,2,4,0;,
  4;24,3,8,9;,
  4;6,5,25,10;,
  4;26,1,26,26;,
  4;18,3,24,27;,
  4;7,10,13,14;,
  4;20,7,14,19;,
  4;8,21,22,11;,
  4;10,25,12,12;,
  4;9,8,11,28;,
  4;4,6,10,7;,
  4;0,4,7,20;,
  4;3,18,21,8;,
  4;29,30,31,21;,
  3;17,30,18;,
  3;32,33,34;,
  3;33,35,34;,
  3;34,35,36;,
  3;35,37,36;,
  3;38,39,31;,
  3;22,21,31;,
  4;40,40,40,40;,
  4;21,31,38,22;;
 }
 MeshTextureCoords {
  32;
  0.564020;0.259270;,
  0.487830;0.338240;,
  0.459460;0.403400;,
  0.447250;0.377280;,
  0.461470;0.416290;,
  0.495650;0.414550;,
  0.485820;0.434230;,
  0.459110;0.472790;,
  0.525950;0.424990;,
  0.534510;0.310270;,
  0.544480;0.359140;,
  0.539670;0.361020;,
  0.529960;0.369470;,
  0.634190;0.322670;,
  0.563620;0.209520;,
  0.487240;0.139420;,
  0.516950;0.124920;,
  0.536830;0.168800;,
  0.649120;0.205390;,
  0.577450;0.180840;,
  0.628010;0.074810;,
  0.687110;0.122040;,
  0.608970;0.151610;,
  0.593350;0.113710;,
  0.524230;0.053470;,
  0.550680;0.091140;,
  0.534330;0.377910;,
  0.531570;0.388280;,
  0.555660;0.362860;,
  0.638640;0.435080;,
  0.651420;0.426520;,
  0.680910;0.368700;;
 }
}
