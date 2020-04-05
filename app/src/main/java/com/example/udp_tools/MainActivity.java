package com.example.udp_tools;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    static int echoSequence = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        Button configButton = findViewById(R.id.config_button);
        Button bandwidthButton = findViewById(R.id.bandwidth_button);
        Button generatorButton = findViewById(R.id.generate_data_button);
        Button echoButton = findViewById(R.id.echo_button);
        Button interactiveButton = findViewById(R.id.interactive_button);

        TextView output = findViewById(R.id.output);
        output.setMovementMethod(new ScrollingMovementMethod());


            // automatically bind preset address and port
        TextView messageField = findViewById(R.id.message_field);
        // getting preset ip address and port
        LayoutInflater inflater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View vi = inflater.inflate(R.layout.activity_configuration, null);
        EditText ipAddress = (EditText) vi.findViewById(R.id.ip_address);
        EditText port = (EditText) vi.findViewById(R.id.port);
        String ipStr = ipAddress.getText().toString();
        int portInt = Integer.parseInt(port.getText().toString());
        // bind port
        int status = bindFromJNI(ipStr, portInt);
        if (status == 0) {
            messageField.setText("Binding was successful!");
        } else {
            messageField.setText("Port is already bound or binding failed!");
        }


//        // initialize EGL
//        EGL10 egl = (EGL10) EGLContext.getEGL();
//        EGLDisplay display = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
//        boolean ret = egl.eglInitialize(display, null);
//
//        if (!ret) {
//            System.out.println("EGL init error: " + egl.eglGetError());
//        }


        // go to InteractiveActivity when interactive button is clicked
        interactiveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent interactiveActivityIntent = new Intent(MainActivity.this, InteractiveActivity.class);
                startActivity(interactiveActivityIntent);
            }
        });

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
//                        // initialize EGL
//                        EGL10 egl = (EGL10) EGLContext.getEGL();
//                        EGLDisplay display = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
//                        boolean ret = egl.eglInitialize(display, null);
//
//                        if (!ret) {
//                            System.out.println("EGL init error: " + egl.eglGetError());
//                        }
//                        TextView output = findViewById(R.id.output);
                        LayoutInflater inflater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                        View vi = inflater.inflate(R.layout.activity_configuration, null);
                        EditText ipAddress = (EditText) vi.findViewById(R.id.ip_address);
                        EditText port = (EditText) vi.findViewById(R.id.port);
                        String ipStr = ipAddress.getText().toString();
                        int portInt = Integer.parseInt(port.getText().toString());

//                        // initialize EGL
//                        EGL10 egl = (EGL10) EGLContext.getEGL();
//                        EGLDisplay display = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
//                        boolean ret = egl.eglInitialize(display, null);

//                        if (!ret) {
//                            System.out.println("EGL init error: " + egl.eglGetError());
//                        }
                        System.out.println("reached here");
                        bandwidthFromJNI(ipStr, portInt);

//                output.append("\n" +bandwidthlOut);
//                        System.out.println(result);
                    }
                }).start();

            }
        });

        // send bandwidth measurement packets when clicking on the bandwidth button
        generatorButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
//                TextView messageField = findViewById(R.id.message_field);
//                messageField.setText("Data stream has been generated!");



                new Thread(new Runnable() {
                    @Override
                    public void run() {
//                        // initialize EGL
//                        EGL10 egl = (EGL10) EGLContext.getEGL();
//                        EGLDisplay display = egl.eglGetDisplay(EGL10.EGL_DEFAULT_DISPLAY);
//                        boolean ret = egl.eglInitialize(display, null);
//
//                        if (!ret) {
//                            System.out.println("EGL init error: " + egl.eglGetError());
//                        }
                        generateDataFromJNI();
                        System.out.println("data stream has been generated!");
                    }
                }).start();

//                AsyncTask.execute(new Runnable() {
//                    @Override
//                    public void run() {
//
//                        //TODO your background code
//                    }
//                });
            }
        });





        echoButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                TextView output = findViewById(R.id.output);
                LayoutInflater inflater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                View vi = inflater.inflate(R.layout.activity_configuration, null);
                EditText ipAddress = (EditText) vi.findViewById(R.id.ip_address);
                EditText port = (EditText) vi.findViewById(R.id.port);
                String ipStr = ipAddress.getText().toString();
                int portInt = Integer.parseInt(port.getText().toString());

                String RTT;
                RTT = echoFromJNI(ipStr, portInt, ++echoSequence);

                output.append("\n" + RTT);
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