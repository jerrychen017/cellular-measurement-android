package com.example.udp_tools;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    boolean start = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);



        FloatingActionButton button = findViewById(R.id.send_button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                start = true;
                TextView tv = findViewById(R.id.sample_text);
                String RTT;
                if (start) {
                   RTT = resendFromJNI();
                } else {
                   RTT = stringFromJNI();
                }
                tv.setText(RTT);
                System.out.println(RTT);
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String resendFromJNI();
}
