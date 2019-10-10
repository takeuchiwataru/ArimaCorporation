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
 26;
 66.23021;9.41680;-5.19702;,
 -68.31558;9.41680;-5.19702;,
 -68.31558;116.35047;-5.19702;,
 66.23021;116.35047;-5.19702;,
 66.23021;116.35047;5.08957;,
 -68.31558;116.35047;5.08957;,
 -68.31558;9.41680;5.08957;,
 66.23021;9.41680;5.08957;,
 -55.56748;16.99291;-8.05280;,
 0.05118;8.05817;-8.17891;,
 0.05118;7.84912;6.63228;,
 -55.56748;16.78387;6.75840;,
 55.78773;16.55305;-8.05901;,
 55.78773;16.34398;6.75220;,
 71.22348;63.07913;-7.40230;,
 71.22348;62.87004;7.40890;,
 48.18270;99.94201;-6.88199;,
 48.18270;99.73298;7.92920;,
 0.07499;117.70781;-6.63122;,
 0.07499;117.49885;8.17997;,
 -42.31117;110.16381;-6.73771;,
 -42.31117;109.95480;8.07348;,
 -71.31339;69.44312;-7.31247;,
 -71.31339;69.23407;7.49872;,
 -55.56748;16.99291;-8.05280;,
 -55.56748;16.78387;6.75840;;
 
 10;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;9,12,13,10;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;16,18,19,17;,
 4;18,20,21,19;,
 4;20,22,23,21;,
 4;22,24,25,23;;
 
 MeshMaterialList {
  1;
  10;
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
    "C:\\Users\\game107\\Desktop\\ŠÏ——ŽÔ2.png";
   }
  }
 }
 MeshNormals {
  12;
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  -0.158625;-0.987241;-0.013934;,
  -0.004018;-0.999892;-0.014113;,
  0.150686;-0.988483;-0.013952;,
  0.992912;0.118843;0.001677;,
  0.631124;0.775604;0.010945;,
  0.088700;0.995959;0.014053;,
  -0.534621;0.845008;0.011924;,
  -0.986646;0.162863;0.002298;,
  0.949138;-0.314829;-0.004444;,
  -0.957780;-0.287475;-0.004057;;
  10;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;10,5,5,10;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,9,9,8;,
  4;9,11,11,9;;
 }
 MeshTextureCoords {
  26;
  -0.000722;0.999286;,
  0.716603;0.999286;,
  0.716603;-0.001749;,
  -0.000722;-0.001749;,
  -0.001199;-0.002007;,
  0.714003;-0.002007;,
  0.714003;1.000784;,
  -0.001199;1.000784;,
  0.375000;0.687500;,
  0.406250;0.687500;,
  0.406250;0.311560;,
  0.375000;0.311560;,
  0.437500;0.687500;,
  0.437500;0.311560;,
  0.468750;0.687500;,
  0.468750;0.311560;,
  0.500000;0.687500;,
  0.500000;0.311560;,
  0.531250;0.687500;,
  0.531250;0.311560;,
  0.562500;0.687500;,
  0.562500;0.311560;,
  0.593750;0.687500;,
  0.593750;0.311560;,
  0.625000;0.687500;,
  0.625000;0.311560;;
 }
}
