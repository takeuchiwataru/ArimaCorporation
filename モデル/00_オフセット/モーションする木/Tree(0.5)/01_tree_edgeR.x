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
 -19.74733;5.36695;-7.99728;,
 -22.48888;7.15125;11.09867;,
 12.06643;172.34940;18.14127;,
 12.87338;171.93214;12.58512;,
 -7.42797;7.31445;23.08833;,
 16.48373;172.39931;21.63137;,
 10.55323;5.69390;16.02982;,
 21.70798;172.03180;19.56542;,
 13.41037;3.90505;-3.08533;,
 22.51482;171.61455;14.00927;,
 -1.69708;3.74205;-15.10213;,
 18.09753;171.56480;10.51917;,
 -19.74733;5.36695;-7.99728;,
 12.87338;171.93214;12.58512;,
 -18.18122;-0.81365;-5.92303;,
 -2.43638;-2.23105;-12.12043;,
 -5.48792;-10.36370;5.36982;,
 -7.42797;7.31445;23.08833;,
 -22.48888;7.15125;11.09867;,
 -20.57277;0.74270;10.73422;,
 -7.43527;0.88510;21.19257;,
 10.55323;5.69390;16.02982;,
 8.24948;-0.52845;15.03557;,
 13.41037;3.90505;-3.08533;,
 10.74178;-2.08890;-1.63833;,
 -1.69708;3.74205;-15.10213;,
 -19.74733;5.36695;-7.99728;;
 
 18;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 3;14,15,16;,
 4;17,18,19,20;,
 4;21,17,20,22;,
 4;23,21,22,24;,
 4;23,24,15,25;,
 4;14,26,25,15;,
 4;18,26,14,19;,
 3;19,14,16;,
 3;16,22,20;,
 3;24,22,16;,
 3;15,24,16;,
 3;19,16,20;;
 
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
  -0.788759;-0.079931;-0.609483;,
  -0.753794;0.223876;-0.617798;,
  -0.965327;0.208577;-0.156971;,
  -0.989749;-0.036271;-0.138137;,
  -0.619141;0.096496;0.779329;,
  -0.338983;-0.149934;0.928768;,
  0.363145;-0.044191;0.930684;,
  0.604961;-0.254681;0.754427;,
  0.986085;-0.069249;0.151132;,
  0.934423;-0.324997;-0.145706;,
  0.156375;0.130954;-0.978978;,
  0.140775;-0.171627;-0.975052;,
  -0.623899;-0.667332;-0.406716;,
  -0.271816;-0.958101;-0.090326;,
  0.069595;-0.731658;-0.678110;,
  -0.145784;-0.592412;0.792335;,
  -0.731428;-0.597535;0.328581;,
  0.547047;-0.654889;0.521401;,
  0.654479;-0.725481;-0.212921;,
  -0.620364;-0.064823;0.781631;,
  0.620252;0.046199;-0.783041;,
  0.342809;-0.856981;0.384793;;
  18;
  4;0,3,2,1;,
  4;19,5,4,4;,
  4;6,7,6,6;,
  4;8,9,8,8;,
  4;20,11,10,20;,
  4;11,0,1,10;,
  3;12,14,13;,
  4;5,19,16,15;,
  4;7,5,15,17;,
  4;9,7,17,18;,
  4;9,18,14,11;,
  4;12,0,11,14;,
  4;3,0,12,16;,
  3;16,12,13;,
  3;21,17,15;,
  3;18,17,21;,
  3;14,18,13;,
  3;16,13,15;;
 }
 MeshTextureCoords {
  27;
  0.375000;0.758540;,
  0.416670;0.758440;,
  0.416670;0.530720;,
  0.375000;0.530720;,
  0.458330;0.758460;,
  0.458330;0.530720;,
  0.500000;0.758580;,
  0.500000;0.530720;,
  0.541670;0.758680;,
  0.541670;0.530720;,
  0.583330;0.758660;,
  0.583330;0.530720;,
  0.625000;0.758540;,
  0.625000;0.530720;,
  0.749850;0.713290;,
  0.624570;0.769310;,
  0.499690;0.713380;,
  0.249910;0.602060;,
  0.747710;0.602110;,
  0.623700;0.657750;,
  0.374800;0.657720;,
  0.000000;0.713580;,
  0.249850;0.713480;,
  0.250260;0.825230;,
  0.374980;0.769310;,
  0.749450;0.825230;,
  1.000000;0.713190;;
 }
}
