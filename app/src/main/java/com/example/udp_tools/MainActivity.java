package com.example.udp_tools;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    static int echoSequence = 0;
    static Handler staticHandler;
    TextView output;

    // variables for interaction
    static int counter;
    static int num_dropped;
    static TextView counterView;
    static TextView numDroppedView;
    static TextView latencyView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        Button configButton = findViewById(R.id.config_button);
        Button bandwidthButton = findViewById(R.id.bandwidth_button);
        Button echoButton = findViewById(R.id.echo_button);

        // initialize output TextView
        output = findViewById(R.id.output);
        output.setMovementMethod(new ScrollingMovementMethod());


        // automatically bind preset address and port
        // getting preset ip address and port
        LayoutInflater inflater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View vi = inflater.inflate(R.layout.activity_configuration, null);
        EditText ipAddress = (EditText) vi.findViewById(R.id.ip_address);
        EditText port = (EditText) vi.findViewById(R.id.bandwidth_port);
        String ipStr = ipAddress.getText().toString();
        int portInt = Integer.parseInt(port.getText().toString());
        // bind port
        int status = bindFromJNI(ipStr, portInt);
        if (status == 0) {
            output.append("Binding was successful!");
        } else {
            output.append("Port is already bound or binding failed!");
        }


        // go to ConfigurationActivity when config button is clicked
        configButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent configActivityIntent = new Intent(MainActivity.this, ConfigurationActivity.class);
                startActivity(configActivityIntent);
            }
        });

        // send bandwidth measurement packets when clicking on the bandwidth button
        bandwidthButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        LayoutInflater inflater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                        View vi = inflater.inflate(R.layout.activity_configuration, null);
                        EditText ipAddress = (EditText) vi.findViewById(R.id.ip_address);
                        EditText port = (EditText) vi.findViewById(R.id.bandwidth_port);
                        String ipStr = ipAddress.getText().toString();
                        int portInt = Integer.parseInt(port.getText().toString());

                        bandwidthFromJNI(ipStr, portInt);

                    }
                }).start();
                try {
                    TimeUnit.SECONDS.sleep(1);
                } catch (Exception e) {

                }
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        generateDataFromJNI();
                        System.out.println("data stream has been generated!");
                    }
                }).start();

            }
        });

        echoButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                TextView output = findViewById(R.id.output);
                LayoutInflater inflater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                View vi = inflater.inflate(R.layout.activity_configuration, null);
                EditText ipAddress = (EditText) vi.findViewById(R.id.ip_address);
                EditText port = (EditText) vi.findViewById(R.id.interactive_port);
                String ipStr = ipAddress.getText().toString();
                int portInt = Integer.parseInt(port.getText().toString());

                String RTT;
                RTT = echoFromJNI(ipStr, portInt, ++echoSequence);

                output.append("\n" + RTT);
                System.out.println(RTT);
            }
        });
        staticHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                output.append("\n" + new String( msg.getData().getCharArray("feedback")));
            }
        };

        // for interaction
        Button connectButton = findViewById(R.id.interactive_connect_button);
        connectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                InteractiveView interactiveView = findViewById(R.id.interactiveView);
                EditText name = findViewById(R.id.interactive_name);
                LayoutInflater inflater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                View vi = inflater.inflate(R.layout.activity_configuration, null);
                EditText ipAddress = (EditText) vi.findViewById(R.id.ip_address);
                EditText port = (EditText) vi.findViewById(R.id.interactive_port);
                String ipStr = ipAddress.getText().toString();
                int portInt = Integer.parseInt(port.getText().toString());
                interactiveView.connect(ipStr, portInt, name.getText().toString());
            }
        });

        counterView =  findViewById(R.id.counter_view);
        numDroppedView = findViewById(R.id.num_dropped_view);
        latencyView = findViewById(R.id.latency_view);

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        staticHandler = null;
    }


    public void feedbackMessage(String s) {
//        Log.d("C++ feedback", "java called!");
        Message msg = new Message();
        Bundle bundle = new Bundle();
        bundle.putCharArray("feedback", s.toCharArray());
        msg.setData(bundle);
        staticHandler.sendMessage(msg);
    }

    public static void updateStat(int num_count, int num_dropped_packet, double latency) {
        counter = num_count;
        num_dropped = num_dropped_packet;
        counterView.setText("Counter: " + counter);
        numDroppedView.setText("Num Dropped: " + num_dropped);
        latencyView.setText("Latency: " + String.format("%.2f", latency) + " ms");
    }

    public native int bandwidthFromJNI(String ip, int port);
    public native void generateDataFromJNI();

    /**
     * Binds the port to the address
     * @param ip destination port
     * @param port destination address
     * @return 1 representing success or 0 representing failure
     */
    public native int bindFromJNI(String ip, int port);

    public native String echoFromJNI(String ip, int port, int seq);

}