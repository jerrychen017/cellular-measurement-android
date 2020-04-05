package com.example.udp_tools;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

public class InteractiveActivity extends AppCompatActivity {
    static int counter;
    static int num_dropped;
    static TextView counterView;
    static TextView numDroppedView;
    static TextView latencyView;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_interactive);

        FloatingActionButton gobackButton = findViewById(R.id.goback_button);
        gobackButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent mainActivityIntent = new Intent(InteractiveActivity.this, MainActivity.class);
                startActivity(mainActivityIntent);
            }
        });

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

    public static void updateStat(int num_count, int num_dropped_packet, double latency) {
        counter = num_count;
        num_dropped = num_dropped_packet;
        counterView.setText("Counter: " + counter);
        numDroppedView.setText("Num Dropped: " + num_dropped);
        latencyView.setText("Latency: " + String.format("%.2f", latency) + " ms");
    }

}


