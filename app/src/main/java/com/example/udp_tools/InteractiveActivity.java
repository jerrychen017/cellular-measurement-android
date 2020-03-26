package com.example.udp_tools;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import com.google.android.material.floatingactionbutton.FloatingActionButton;

public class InteractiveActivity extends AppCompatActivity {
    static int counter;
    static int num_dropped;
    static TextView counterView;
    static TextView numDroppedView;

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
        counterView =  findViewById(R.id.counter_view);
        numDroppedView = findViewById(R.id.num_dropped_view);
    }

    public static void setCounter(int num_count, int num_dropped_packet) {
        counter = num_count;
        num_dropped = num_dropped_packet;
        counterView.setText("Counter: " + counter);
        numDroppedView.setText("Num Dropped: " + num_dropped);
    }
}
