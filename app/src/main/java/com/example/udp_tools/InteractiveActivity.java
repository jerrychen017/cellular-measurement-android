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
    static TextView counterView;

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
    }

    public static void setCounter(int n) {
        counter = n;
        counterView.setText("Counter: " + counter);
    }
}
