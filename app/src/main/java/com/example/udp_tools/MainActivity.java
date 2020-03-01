package com.example.udp_tools;

import androidx.appcompat.app.AppCompatActivity;

import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        FloatingActionButton configButton = findViewById(R.id.config_button);
        FloatingActionButton interarrivalButton = findViewById(R.id.interarrival_button);
        FloatingActionButton echoButton = findViewById(R.id.echo_button);
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

        interarrivalButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                TextView output = findViewById(R.id.output);
                EditText ipAddress = (EditText) findViewById(R.id.ip_address);
                EditText port = (EditText) findViewById(R.id.port);
                String ipStr = ipAddress.getText().toString();
                int portInt = Integer.parseInt(port.getText().toString());

                String RTT;
                RTT = interarrivalFromJNI(ipStr, portInt);

                output.setText( RTT);
                System.out.println(RTT);
            }
        });

        echoButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                TextView output = findViewById(R.id.output);
                EditText ipAddress = (EditText) findViewById(R.id.ip_address);
                EditText port = (EditText) findViewById(R.id.port);
                String ipStr = ipAddress.getText().toString();
                int portInt = Integer.parseInt(port.getText().toString());

                String RTT;
                RTT = echoFromJNI(ipStr, portInt);

                output.setText( RTT);
                System.out.println(RTT);
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     *
     * Calls a cpp function to send interarrival packets to the server
     */
    public native String interarrivalFromJNI(String ip, int port);

    /**
     * Binds the port to the address
     * @param ip destination port
     * @param port destination address
     * @return 1 representing success or 0 representing failure
     */
    public native int bindFromJNI(String ip, int port);

    public native String echoFromJNI(String ip, int port);
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