package opengl.panjq.com.opengl_demo.Render;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Shader;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class ShaderRender implements GLSurfaceView.Renderer {
    static {
        System.loadLibrary("shader-lib");
    }

    private AssetManager assetManager;
    private static final String TAG = "ShaderRender";

    public native void load();

    public native void glesInit(AssetManager assetManager);

    public native void glesResize(int width, int height);

    public native void glesRender();

    public native void glesInitUniform(AssetManager assetManager);

    public native void glesRenderUniform();

    public native void glesInitMultiAttribute(AssetManager assetManager);

    public native void glesRenderMultiAttribute();


    public ShaderRender(Context context) {
        assetManager = context.getAssets();
    }


    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        glesInitMultiAttribute(assetManager);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int width, int height) {
        glesResize(width,height);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        glesRenderMultiAttribute();
    }
}
