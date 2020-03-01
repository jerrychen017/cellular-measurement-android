package com.example.udp_tools;

import androidx.appcompat.app.AppCompatActivity;

import android.os.AsyncTask;
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
        FloatingActionButton configButton = findViewById(R.id.config_button);
        FloatingActionButton sendButton = findViewById(R.id.send_button);
//        AsyncTask myAsyncTask;

        configButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //bind port
                TextView tv = findViewById(R.id.sample_text);

                EditText ipAddress = (EditText) findViewById(R.id.ip_address);
                EditText port = (EditText) findViewById(R.id.port);
                String ipStr = ipAddress.getText().toString();
                int portInt = Integer.parseInt(port.getText().toString());

                int status = bindFromJNI(ipStr, portInt);

                if (status == 0) {
                    tv.setText("Binding was successful!");
                } else {
                    tv.setText("Binding failed!");
                }
            }
        });

        sendButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                TextView output = findViewById(R.id.output);
                EditText ipAddress = (EditText) findViewById(R.id.ip_address);
                EditText port = (EditText) findViewById(R.id.port);
                String ipStr = ipAddress.getText().toString();
                int portInt = Integer.parseInt(port.getText().toString());

                String RTT;
                RTT = sendFromJNI(ipStr, portInt);

                output.setText( RTT);
                System.out.println(RTT);
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String sendFromJNI(String ip, int port);

    public native int bindFromJNI(String ip, int port);
}

class MyAsyncTask extends AsyncTask<String, String, String> {


    @Override
    protected String doInBackground(String... strings) {
        return null;
    }

    @Override
    protected void onPostExecute(String string) {
        super.onPostExecute(string);
    }

    @Override
    protected void onProgressUpdate(String... values) {
        super.onProgressUpdate(values);
    }
}