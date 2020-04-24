package com.example.udp_tools;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.jjoe64.graphview.GraphView;

import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.util.Calendar;
import java.util.Date;

public class MainActivity extends AppCompatActivity {

    static class GraphHandler extends Handler {
        private LineGraphSeries<DataPoint> data;
        private GraphView graph;
        private String fieldName;
        private Date startTime;

        public GraphHandler(LineGraphSeries<DataPoint> data, GraphView graph, String fieldName) {
            this.data = data;
            this.graph = graph;
            this.fieldName = fieldName;
        }

        @Override
        public void handleMessage(Message msg) {
            Calendar calendar = Calendar.getInstance();
            Date d = calendar.getTime();
            double x = (d.getTime() - startTime.getTime()) / 1000.0;
            double bw = msg.getData().getDouble(fieldName);
            data.appendData(new DataPoint(x, bw), true, 1000);
            graph.invalidate();
        }

        public void setStartTime(Date time) {
            startTime = time;
        }
    }

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    static int echoSequence = 0;
    static GraphHandler uploadHandler;
    static GraphHandler downloadHandler;
    TextView output;

    // variables for interaction
    static int counter;
    static int num_dropped;
    static TextView counterView;
    static TextView numDroppedView;
    static TextView latencyView;
    private boolean connected = false;

    private GraphView graph;
    private LineGraphSeries<DataPoint> uploadData;
    private LineGraphSeries<DataPoint> downloadData;
    private Date startTime;

    private Parameters params; // bandwidth parameters

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        Button configButton = findViewById(R.id.config_button);
        Button bandwidthButton = findViewById(R.id.bandwidth_button);
        Button bandwidthStopButton = findViewById(R.id.bandwidth_stop_button);
        Button echoButton = findViewById(R.id.echo_button);

        // initialize output TextView
        output = findViewById(R.id.output);
        output.setMovementMethod(new ScrollingMovementMethod());

        // Setup graph
        graph = findViewById(R.id.graph);
        uploadData = new LineGraphSeries<>(new DataPoint[]{});
        downloadData = new LineGraphSeries<>(new DataPoint[]{});
        graph.addSeries(uploadData);
        graph.addSeries(downloadData);
        downloadData.setColor(Color.BLUE);
        uploadData.setColor(Color.RED);
        graph.getViewport().setMinX(0);
        graph.getViewport().setMaxX(10);
        graph.getViewport().setXAxisBoundsManual(true);
        graph.getViewport().setScrollable(true);
        graph.getViewport().setMaxY(15);
        graph.getViewport().setYAxisBoundsManual(true);

        // Append to graph on message
        uploadHandler = new GraphHandler(uploadData, graph, "feedbackUpload");
        downloadHandler = new GraphHandler(downloadData, graph, "feedbackDownload");

        // initialize parameters
        params = new Parameters(10, 500, 0, 0, 2, 0.1, 10, 1, 10);


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
                stopDataGeneratorThreadFromJNI();
                stopControllerThreadFromJNI();
                stopReceivingThreadFromJNI();
                // start sending
                System.out.println("bandwidth is started!");
                startTime = Calendar.getInstance().getTime();
                uploadHandler.setStartTime(startTime);
                downloadHandler.setStartTime(startTime);
                uploadData.resetData(new DataPoint[]{});
                downloadData.resetData(new DataPoint[]{});

                // start handshake process
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        LayoutInflater inflater = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                        View vi = inflater.inflate(R.layout.activity_configuration, null);
                        EditText ipAddress = (EditText) vi.findViewById(R.id.ip_address);
                        String ipStr = ipAddress.getText().toString();
                        startClientAndroidFromJNI(ipStr, params);

                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                LayoutInflater inflater = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                                View vi = inflater.inflate(R.layout.activity_configuration, null);
                                EditText ipAddress = (EditText) vi.findViewById(R.id.ip_address);
                                String ipStr = ipAddress.getText().toString();
                                receiveBandwidthFromJNI(ipStr, 1, params);
                            }
                        }).start();

                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                LayoutInflater inflater = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                                View vi = inflater.inflate(R.layout.activity_configuration, null);
                                EditText ipAddress = (EditText) vi.findViewById(R.id.ip_address);
                                String ipStr = ipAddress.getText().toString();
                                startControllerFromJNI(ipStr, params);
                            }
                        }).start();

                        new Thread(new Runnable() {
                            @Override
                            public void run() {
                                startDataGeneratorFromJNI();
                            }
                        }).start();
                    }
                }).start();

            }
        });


        bandwidthStopButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // stop bandwidth thread
                stopDataGeneratorThreadFromJNI();
                stopControllerThreadFromJNI();
                stopReceivingThreadFromJNI();
                output.append("bandwidth measurement stopped\n");
            }
        });

        echoButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                TextView output = findViewById(R.id.output);
                LayoutInflater inflater = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                View vi = inflater.inflate(R.layout.activity_configuration, null);
                EditText ipAddress = (EditText) vi.findViewById(R.id.ip_address);
                EditText port = (EditText) vi.findViewById(R.id.interactive_port);
                String ipStr = ipAddress.getText().toString();
                int portInt = Integer.parseInt(port.getText().toString());

                String RTT;
                RTT = echoFromJNI(ipStr, portInt, ++echoSequence);

                output.append(RTT + "\n");
                System.out.println(RTT);
            }
        });


        // for interaction
        Button connectButton = findViewById(R.id.interactive_connect_button);
        connectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (connected) {
                    output.append("Connected already\n");
                    return;
                }
                InteractiveView interactiveView = findViewById(R.id.interactiveView);
                EditText name = findViewById(R.id.interactive_name);
                LayoutInflater inflater = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                View vi = inflater.inflate(R.layout.activity_configuration, null);
                EditText ipAddress = (EditText) vi.findViewById(R.id.ip_address);
                EditText port = (EditText) vi.findViewById(R.id.interactive_port);
                String ipStr = ipAddress.getText().toString();
                int portInt = Integer.parseInt(port.getText().toString());
                interactiveView.connect(ipStr, portInt, name.getText().toString());
                connected = true;
                output.append("Connected\n");
                name.setText("");
                name.clearFocus();
            }
        });

        counterView = findViewById(R.id.counter_view);
        numDroppedView = findViewById(R.id.num_dropped_view);
        latencyView = findViewById(R.id.latency_view);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        uploadHandler = null;
    }

    public void sendFeedbackUpload(double d) {
        Message msg = new Message();
        Bundle bundle = new Bundle();
        bundle.putDouble("feedbackUpload", d);
        msg.setData(bundle);
        uploadHandler.sendMessage(msg);
    }

    public void sendFeedbackDownload(double d) {
        Message msg = new Message();
        Bundle bundle = new Bundle();
        bundle.putDouble("feedbackDownload", d);
        msg.setData(bundle);
        downloadHandler.sendMessage(msg);
    }

    @SuppressLint("DefaultLocale")
    public static void updateStat(int num_count, int num_dropped_packet, double latency) {
        counter = num_count;
        num_dropped = num_dropped_packet;
        counterView.setText("Counter: " + counter);
        numDroppedView.setText("Num Dropped: " + num_dropped);
        latencyView.setText("Latency: " + String.format("%.2f", latency) + " ms");
    }

    public native String echoFromJNI(String ip, int port, int seq);

    public native void stopDataGeneratorThreadFromJNI();

    public native void stopControllerThreadFromJNI();

    public native void stopReceivingThreadFromJNI();

    public native void startControllerFromJNI(String ip, Parameters params);

    public native void startDataGeneratorFromJNI();

    public native void startClientAndroidFromJNI(String ip, Parameters params);

    public native void receiveBandwidthFromJNI(String ip, int predMode, Parameters params);

}