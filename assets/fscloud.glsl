#version 400
uniform vec3 EyePos;
uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

uniform vec3 Scaling;

in vec3 Position;
in vec3 Pos;
in vec3 Normal;
in vec2 Texcoord;
out vec4 FragColor;

uniform sampler2D WorleyNoise;


void main()
{

    /*
    
    Vector Application::calc3DRay( float x, float y, Vector& Pos)
    {
    Matrix view, proj;
    view = Cam.getViewMatrix();
    view.invert();
    proj = Cam.getProjectionMatrix();
    proj.invert();

    Vector v = proj.transformVec4x4(Vector(x, y, 0));
    Pos = view.translation();

    return view.transformVec3x3(v);
    }

        Vector r;                      //g = r + s * d
        Vector d = calc3DRay(x, y, r); //vector wird in r geschrieben

        float s = -r.Y / d.Y; //r.y + s * d.y = 0, nach s umstellen
        Vector mousePosInSpace = r + (d * s); <- THIS

    */
    


    float NormScreenCoordX = gl_FragCoord.x / 1920.0f;
    float NormScreenCoordY = gl_FragCoord.y / 1080.0f;

    //MVC-Matrix

    //CalcWorldSpace of Ray @ Pixel w/ y=top -> UV-Coords -- WorldSpaceTop
    //CalcWorldSpace of Ray @ Pixel w/ y=bottom -> UV-Coords -- WorldSpaceBottom

    /*
    float AlfaIntensity=0;
    for(int i=0;i>128;i++){
        AlfaIntensity = texture(WorleyNoise[i], WorldSpaceBottom.x+((WorldSpaceTop.x - WorldSpaceBottom.x)/128)*i, WorldSpaceBottom.y+((WorldSpaceTop.y - WorldSpaceBottom.y)/128)*i).r;
    }
    AlfaIntensity/=128; <- Transparent
    */
    vec2 ScreenCoord = vec2(NormScreenCoordX,NormScreenCoordY);

    vec3 col;
    col = texture(WorleyNoise, ScreenCoord).rgb;

    FragColor = vec4(col,col.r);

}
