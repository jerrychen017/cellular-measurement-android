package com.example.udp_tools;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    // first UDP packet has been sent
    boolean initSend = false;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);



        FloatingActionButton button = findViewById(R.id.send_button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                initSend = true;
                TextView tv = findViewById(R.id.sample_text);

                TextView output = findViewById(R.id.output);

                EditText ipAddress = (EditText) findViewById(R.id.ip_address);
                EditText port = (EditText) findViewById(R.id.port);
                String ipStr = ipAddress.getText().toString();
                int portInt = Integer.parseInt(port.getText().toString());

                String RTT;
                if (initSend) {
                   RTT = resendFromJNI(ipStr, portInt);
                } else {
                   RTT = initSendFromJNI(ipStr, portInt);
                }
                tv.setText(RTT);
                output.setText(output.getText().toString()  + "\n" + RTT);
                System.out.println(RTT);
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String initSendFromJNI(String ip, int port);

    public native String resendFromJNI(String ip, int port);
}
