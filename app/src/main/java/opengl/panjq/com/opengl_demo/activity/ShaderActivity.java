package opengl.panjq.com.opengl_demo.activity;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import opengl.panjq.com.opengl_demo.Render.ShaderRender;


public class ShaderActivity extends AppCompatActivity {
    private GLSurfaceView glSurfaceView;
    private final int GL_VERSION = 3;
    private ShaderRender renderer;
    private static final String TAG = "ShaderActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        glSurfaceView = new GLSurfaceView(this);
        renderer = new ShaderRender(this);
        if (detectOpenGLES30()) {
            glSurfaceView.setEGLContextClientVersion(GL_VERSION);
            glSurfaceView.setRenderer(renderer);
            glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        } else {
            Log.e(TAG, "OpenGL ES 3.0 is not supported in this device ");
            finish();
        }
        setContentView(glSurfaceView);
    }

    private boolean detectOpenGLES30() {
        ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        return (info.reqGlEsVersion >= 0x30000);
    }

    @Override
    protected void onPause() {
        super.onPause();
        glSurfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        glSurfaceView.onResume();
    }
}
