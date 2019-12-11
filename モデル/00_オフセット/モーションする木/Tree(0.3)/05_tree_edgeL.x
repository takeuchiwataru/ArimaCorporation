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
 27;
 8.43523;0.91316;-7.53808;,
 -11.13717;100.85230;4.81137;,
 -10.65297;101.10262;8.14506;,
 10.08022;1.98374;3.91949;,
 -13.30338;101.13256;10.23912;,
 1.04364;2.08165;11.11329;,
 -16.43790;100.91206;8.99955;,
 -9.74505;1.10933;6.87818;,
 -16.92204;100.66170;5.66586;,
 -11.45937;0.03598;-4.59091;,
 -14.27166;100.63186;3.57180;,
 -2.39487;-0.06178;-11.80099;,
 -11.13717;100.85230;4.81137;,
 8.43523;0.91316;-7.53808;,
 7.49562;-2.79523;-6.29353;,
 -0.12036;-8.52526;0.48218;,
 -1.95135;-3.64564;-10.01197;,
 1.04364;2.08165;11.11329;,
 1.04802;-1.77595;9.97583;,
 8.93049;-1.86140;3.70082;,
 10.08022;1.98374;3.91949;,
 -9.74505;1.10933;6.87818;,
 -8.36283;-2.62408;6.28163;,
 -11.45937;0.03598;-4.59091;,
 -9.85821;-3.56035;-3.72271;,
 -2.39487;-0.06178;-11.80099;,
 8.43523;0.91316;-7.53808;;
 
 18;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 3;14,15,16;,
 4;17,18,19,20;,
 4;21,22,18,17;,
 4;23,24,22,21;,
 4;23,25,16,24;,
 4;14,16,25,26;,
 4;20,19,14,26;,
 3;19,15,14;,
 3;15,18,22;,
 3;24,15,22;,
 3;16,15,24;,
 3;19,18,15;;
 
 MeshMaterialList {
  1;
  18;
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
    "tree000.jpg";
   }
  }
 }
 MeshNormals {
  22;
  0.989748;-0.036271;-0.138140;,
  0.788758;-0.079931;-0.609485;,
  0.338981;-0.149935;0.928769;,
  -0.604962;-0.254679;0.754427;,
  -0.934424;-0.324996;-0.145703;,
  -0.140775;-0.171626;-0.975052;,
  0.965326;0.208577;-0.156977;,
  0.753792;0.223875;-0.617801;,
  0.619138;0.096495;0.779331;,
  -0.363147;-0.044191;0.930683;,
  -0.986084;-0.069249;0.151138;,
  -0.156375;0.130954;-0.978978;,
  0.623899;-0.667332;-0.406716;,
  -0.547048;-0.654888;0.521402;,
  0.731428;-0.597534;0.328582;,
  -0.654480;-0.725480;-0.212921;,
  -0.069596;-0.731657;-0.678111;,
  0.145784;-0.592412;0.792335;,
  0.271817;-0.958101;-0.090326;,
  0.620362;-0.064826;0.781632;,
  -0.620251;0.046199;-0.783042;,
  -0.342810;-0.856980;0.384794;;
  18;
  4;1,7,6,0;,
  4;19,8,8,2;,
  4;9,9,9,3;,
  4;10,10,10,4;,
  4;20,20,11,5;,
  4;5,11,7,1;,
  3;12,18,16;,
  4;2,17,14,19;,
  4;3,13,17,2;,
  4;4,15,13,3;,
  4;4,5,16,15;,
  4;12,16,5,1;,
  4;0,14,12,1;,
  3;14,18,12;,
  3;21,17,13;,
  3;15,21,13;,
  3;16,18,15;,
  3;14,17,18;;
 }
 MeshTextureCoords {
  27;
  0.375000;0.758540;,
  0.375000;0.530720;,
  0.416670;0.530720;,
  0.416670;0.758440;,
  0.458330;0.530720;,
  0.458330;0.758460;,
  0.500000;0.530720;,
  0.500000;0.758580;,
  0.541670;0.530720;,
  0.541670;0.758680;,
  0.583330;0.530720;,
  0.583330;0.758660;,
  0.625000;0.530720;,
  0.625000;0.758540;,
  0.749850;0.713290;,
  0.499690;0.713380;,
  0.624570;0.769310;,
  0.249910;0.602060;,
  0.374800;0.657720;,
  0.623700;0.657750;,
  0.747710;0.602110;,
  0.000000;0.713580;,
  0.249850;0.713480;,
  0.250260;0.825230;,
  0.374980;0.769310;,
  0.749450;0.825230;,
  1.000000;0.713190;;
 }
}
