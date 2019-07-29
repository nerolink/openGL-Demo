package opengl.panjq.com.opengl_demo.activity;

import android.opengl.GLSurfaceView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import opengl.panjq.com.opengl_demo.R;

public class TextureActivity extends AppCompatActivity {
    GLSurfaceView glSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        glSurfaceView = new GLSurfaceView(this);



        setContentView(R.layout.activity_texture);
    }
}
