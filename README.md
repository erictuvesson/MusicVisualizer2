# Music Visualizer

This is the continuation of my other project [erictuvesson/MusicVisualizer](https://github.com/erictuvesson/MusicVisualizer) that was made in C#.

> This project is a technical test which is missing some features.
> Testing whether I could get it working easier and smoother than the C# version which had some technical limitations.

## Shader Scene

|Name|Type|Value|Description|
|---|---|---|---|
|iResolution|vec3|(1280, 800, 32)|Window resolution (width, height, depth)|
|iTime|float||Total time in seconds|
|iTimeDelta|float||Delta time in seconds|
|iFrame|float||Current frame index|
|iMouse|vec4||Mouse input|
|iSampleRate|float|||
|iAudioSum|float|0.0-1.0||
|iAudioTime|float|0.0-1.0|Custom audio time based on many variables|
|iSample|float||Audio FFT spectrum|
