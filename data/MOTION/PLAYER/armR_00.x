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
 64;
 0.48520;-0.01835;-0.00000;,
 0.40857;-0.15904;-0.25469;,
 0.38593;-0.01835;-0.32999;,
 0.48520;-0.01835;-0.00000;,
 0.40858;-0.21731;-0.00000;,
 0.48520;-0.01835;-0.00000;,
 0.40857;-0.15904;0.25469;,
 0.48520;-0.01835;-0.00000;,
 0.36328;-0.01835;0.33754;,
 0.48520;-0.01835;-0.00000;,
 0.40858;0.12233;0.25469;,
 0.48520;-0.01835;-0.00000;,
 0.40858;0.18061;0.00000;,
 0.48520;-0.01835;-0.00000;,
 0.40858;0.12233;-0.25469;,
 0.48520;-0.01835;-0.00000;,
 0.38593;-0.01835;-0.32999;,
 -1.52484;-0.01914;-0.32826;,
 -1.63005;-0.01996;0.00207;,
 -1.52518;0.17287;-0.25540;,
 -1.52400;-0.21140;-0.25705;,
 -1.52034;-0.28615;0.00093;,
 -1.51784;-0.21582;0.26358;,
 -1.51832;-0.01909;0.35177;,
 -1.52098;0.18119;0.26818;,
 -1.52462;0.24834;0.00060;,
 -1.25441;0.32448;-0.45784;,
 -0.56558;0.32129;-0.44462;,
 -0.56558;-0.01835;-0.62879;,
 -1.25441;-0.01835;-0.64748;,
 0.01838;-0.01835;-0.54516;,
 0.01838;-0.27830;-0.38549;,
 -0.56558;-0.35800;-0.44462;,
 -0.56558;-0.01835;-0.62879;,
 0.01838;-0.38598;-0.00000;,
 -0.56558;-0.49868;0.00000;,
 0.01838;-0.27830;0.38549;,
 -0.56558;-0.35800;0.44462;,
 0.01838;-0.01835;0.54516;,
 -0.56558;-0.01835;0.62879;,
 0.01838;0.24160;0.38549;,
 -0.56558;0.32129;0.44462;,
 0.01838;0.34927;0.00000;,
 -0.56558;0.46198;0.00000;,
 0.01838;0.24160;-0.38549;,
 0.01838;-0.01835;-0.54516;,
 -1.24696;0.32445;0.45770;,
 -1.25350;0.46630;-0.00058;,
 -1.24198;-0.01835;0.64715;,
 -1.24147;-0.36113;0.45759;,
 -1.24696;0.32445;0.45770;,
 -1.24574;-0.50314;0.00000;,
 -1.25229;-0.36013;-0.45837;,
 -1.24147;-0.36113;0.45759;,
 -1.24574;-0.50314;0.00000;,
 -1.25441;-0.01835;-0.64748;,
 -1.25441;0.32448;-0.45784;,
 -1.25441;-0.01835;-0.64748;,
 -1.25229;-0.36013;-0.45837;,
 -1.24574;-0.50314;0.00000;,
 -1.24147;-0.36113;0.45759;,
 -1.24198;-0.01835;0.64715;,
 -1.24696;0.32445;0.45770;,
 -1.25350;0.46630;-0.00058;;
 
 80;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 3;17,18,19;,
 3;20,18,17;,
 3;21,18,20;,
 3;22,18,21;,
 3;23,18,22;,
 3;24,18,23;,
 3;25,18,24;,
 3;19,18,25;,
 3;26,27,28;,
 3;26,28,29;,
 3;30,31,32;,
 3;30,32,33;,
 3;31,34,35;,
 3;31,35,32;,
 3;35,34,36;,
 3;35,36,37;,
 3;37,36,38;,
 3;37,38,39;,
 3;38,40,41;,
 3;38,41,39;,
 3;40,42,43;,
 3;40,43,41;,
 3;43,42,44;,
 3;43,44,27;,
 3;27,44,45;,
 3;27,45,28;,
 3;2,1,31;,
 3;2,31,30;,
 3;1,4,34;,
 3;1,34,31;,
 3;34,4,6;,
 3;34,6,36;,
 3;36,6,8;,
 3;36,8,38;,
 3;8,10,40;,
 3;8,40,38;,
 3;10,12,42;,
 3;10,42,40;,
 3;42,12,14;,
 3;42,14,44;,
 3;44,14,16;,
 3;44,16,45;,
 3;46,41,43;,
 3;46,43,47;,
 3;39,48,49;,
 3;39,49,37;,
 3;50,48,39;,
 3;50,39,41;,
 3;51,52,32;,
 3;51,32,35;,
 3;37,53,54;,
 3;37,54,35;,
 3;47,43,27;,
 3;47,27,26;,
 3;33,32,52;,
 3;33,52,55;,
 3;56,57,17;,
 3;56,17,19;,
 3;17,57,58;,
 3;17,58,20;,
 3;20,58,59;,
 3;20,59,21;,
 3;59,60,22;,
 3;59,22,21;,
 3;60,61,23;,
 3;60,23,22;,
 3;23,61,62;,
 3;23,62,24;,
 3;24,62,63;,
 3;24,63,25;,
 3;63,56,19;,
 3;63,19,25;;
 
 MeshMaterialList {
  2;
  80;
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
   0.412000;0.400000;0.467000;1.000000;;
   8.000000;
   0.000000;0.000000;0.000000;;
   0.111240;0.108000;0.126090;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  44;
  0.063742;0.000001;-0.997966;,
  0.085011;-0.774738;-0.626542;,
  0.121424;-0.992601;-0.000000;,
  0.085230;-0.774902;0.626309;,
  0.063741;0.000013;0.997966;,
  0.100574;0.721417;0.685159;,
  0.105492;0.993436;0.044234;,
  0.085231;0.774911;-0.626297;,
  0.299017;-0.119066;-0.946790;,
  0.306524;-0.749597;-0.586641;,
  0.320973;-0.947088;-0.000000;,
  0.308455;-0.747689;0.588062;,
  0.303782;-0.119125;0.945265;,
  0.308452;0.747690;0.588062;,
  0.320971;0.947089;0.000000;,
  0.306521;0.749598;-0.586641;,
  0.690760;-0.000004;-0.723084;,
  0.706888;-0.591859;-0.387315;,
  0.723880;-0.689925;-0.000000;,
  0.711620;-0.573009;0.406519;,
  0.677894;-0.000004;0.735160;,
  0.711621;0.573010;0.406517;,
  0.723878;0.689928;0.000000;,
  0.706889;0.591860;-0.387312;,
  0.999912;-0.000004;0.013281;,
  -0.323113;-0.946359;0.001433;,
  -0.367023;-0.628132;-0.686107;,
  -0.423937;-0.001376;-0.905690;,
  -0.352228;-0.635710;0.686883;,
  -0.302379;0.700240;0.646707;,
  -0.399626;-0.003084;0.916673;,
  -0.372714;0.926669;-0.048667;,
  -0.370127;0.626377;-0.686045;,
  -0.999968;-0.007742;0.001785;,
  -0.822980;-0.001275;-0.568068;,
  -0.807074;0.485936;-0.335407;,
  -0.803243;-0.494084;-0.332689;,
  -0.799572;-0.600563;0.002963;,
  -0.793156;-0.508629;0.334962;,
  -0.810571;-0.010391;0.585548;,
  -0.799099;0.497463;0.337596;,
  -0.807006;0.590511;-0.006178;,
  0.155785;0.517008;0.841685;,
  0.155785;0.517008;-0.841685;;
  80;
  3;24,17,16;,
  3;24,18,17;,
  3;24,19,18;,
  3;24,20,19;,
  3;24,21,20;,
  3;24,22,21;,
  3;24,23,22;,
  3;24,16,23;,
  3;34,33,35;,
  3;36,33,34;,
  3;37,33,36;,
  3;38,33,37;,
  3;39,33,38;,
  3;40,33,39;,
  3;41,33,40;,
  3;35,33,41;,
  3;32,7,0;,
  3;32,0,27;,
  3;8,9,1;,
  3;8,1,0;,
  3;9,10,2;,
  3;9,2,1;,
  3;2,10,11;,
  3;2,11,3;,
  3;3,11,12;,
  3;3,12,4;,
  3;42,13,5;,
  3;12,5,4;,
  3;13,14,6;,
  3;13,6,5;,
  3;6,14,15;,
  3;6,15,7;,
  3;7,15,43;,
  3;7,8,0;,
  3;16,17,9;,
  3;16,9,8;,
  3;17,18,10;,
  3;17,10,9;,
  3;10,18,19;,
  3;10,19,11;,
  3;11,19,20;,
  3;11,20,12;,
  3;20,21,13;,
  3;20,13,12;,
  3;21,22,14;,
  3;21,14,13;,
  3;14,22,23;,
  3;14,23,15;,
  3;15,23,16;,
  3;15,16,8;,
  3;29,5,6;,
  3;29,6,31;,
  3;4,30,28;,
  3;4,28,3;,
  3;29,30,4;,
  3;29,4,5;,
  3;25,26,1;,
  3;25,1,2;,
  3;3,28,25;,
  3;3,25,2;,
  3;31,6,7;,
  3;31,7,32;,
  3;0,1,26;,
  3;0,26,27;,
  3;32,27,34;,
  3;32,34,35;,
  3;34,27,26;,
  3;34,26,36;,
  3;36,26,25;,
  3;36,25,37;,
  3;25,28,38;,
  3;25,38,37;,
  3;28,30,39;,
  3;28,39,38;,
  3;39,30,29;,
  3;39,29,40;,
  3;40,29,31;,
  3;40,31,41;,
  3;31,32,35;,
  3;31,35,41;;
 }
 MeshTextureCoords {
  64;
  0.062500;1.000000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.187500;1.000000;,
  0.250000;0.875000;,
  0.312500;1.000000;,
  0.375000;0.875000;,
  0.437500;1.000000;,
  0.500000;0.875000;,
  0.562500;1.000000;,
  0.625000;0.875000;,
  0.687500;1.000000;,
  0.750000;0.875000;,
  0.812500;1.000000;,
  0.875000;0.875000;,
  0.937500;1.000000;,
  1.000000;0.875000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.875000;0.500000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  1.000000;0.500000;,
  0.000000;0.750000;,
  0.125000;0.750000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.750000;,
  0.250000;0.625000;,
  0.375000;0.750000;,
  0.375000;0.625000;,
  0.500000;0.750000;,
  0.500000;0.625000;,
  0.625000;0.750000;,
  0.625000;0.625000;,
  0.750000;0.750000;,
  0.750000;0.625000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.625000;0.501350;,
  0.750000;0.500170;,
  0.500000;0.502260;,
  0.377310;0.502350;,
  0.623660;0.501360;,
  0.248480;0.501560;,
  0.125000;0.500390;,
  0.375000;0.502350;,
  0.251540;0.501580;,
  0.000000;0.500000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
