package com.example.udp_tools;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class ConfigurationActivity extends AppCompatActivity {
    private SharedPreferences prefs;
    private Editor editor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_configuration);

        prefs = getSharedPreferences("cellular-measurement", MODE_PRIVATE);
        editor = prefs.edit();

        // load parameters from SharedPreferences
        EditText ipStr = (EditText) findViewById(R.id.ip_address);
        String ip = prefs.getString("ip", ipStr.getText().toString());
        ipStr.setText(ip);

        EditText uploadPortStr = (EditText) findViewById(R.id.upload_port);
        String uploadPort = prefs.getString("uploadPort", uploadPortStr.getText().toString());
        uploadPortStr.setText(""+ uploadPort);

        EditText downloadPortStr = (EditText) findViewById(R.id.download_port);
        String downloadPort = prefs.getString("downloadPort", downloadPortStr.getText().toString());
        downloadPortStr.setText("" + downloadPort);

        EditText interactivePortStr = (EditText) findViewById(R.id.interactive_port);
        String interactivePort = prefs.getString("interactivePort", interactivePortStr.getText().toString());
        interactivePortStr.setText("" + interactivePort);

        EditText burstSizeStr = (EditText) findViewById(R.id.burst_size);
        String burstSize = prefs.getString("burstSize", burstSizeStr.getText().toString());
        burstSizeStr.setText("" + burstSize);

        EditText burstFactorStr = (EditText) findViewById(R.id.burst_factor);
        String burstFactor = prefs.getString("burstFactor", burstFactorStr.getText().toString());
        burstFactorStr.setText("" + burstFactor);

        EditText intervalSizeStr = (EditText) findViewById(R.id.interval_size);
        String intervalSize = prefs.getString("intervalSize", intervalSizeStr.getText().toString());
        intervalSizeStr.setText("" + intervalSize);

        EditText intervalTimeStr = (EditText) findViewById(R.id.interval_time);
        String intervalTime = prefs.getString("intervalTime", intervalTimeStr.getText().toString());
        intervalTimeStr.setText("" + intervalTime);

        EditText minSpeedStr = (EditText) findViewById(R.id.min_speed);
        String minSpeed = prefs.getString("minSpeed", minSpeedStr.getText().toString());
        minSpeedStr.setText("" + minSpeed);

        EditText maxSpeedStr = (EditText) findViewById(R.id.max_speed);
        String maxSpeed = prefs.getString("maxSpeed", maxSpeedStr.getText().toString());
        maxSpeedStr.setText("" + maxSpeed);

        EditText startSpeedStr = (EditText) findViewById(R.id.start_speed);
        String startSpeed = prefs.getString("startSpeed", startSpeedStr.getText().toString());
        startSpeedStr.setText("" + startSpeed);

        EditText gracePeriodStr = (EditText) findViewById(R.id.grace_period);
        String gracePeriod = prefs.getString("gracePeriod",gracePeriodStr.getText().toString());
        gracePeriodStr.setText("" + gracePeriod);

        EditText instantBurstStr = (EditText) findViewById(R.id.instant_burst);
        String instantBurst = prefs.getString("instantBurst", instantBurstStr.getText().toString());
        instantBurstStr.setText("" + instantBurst);

        Button saveButton = findViewById(R.id.config_save_button);

        // go to InteractiveActivity when interactive button is clicked
        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                EditText ipStr = (EditText) findViewById(R.id.ip_address);
                editor.putString("ip", ipStr.getText().toString());

                EditText uploadPortStr = (EditText) findViewById(R.id.upload_port);
                editor.putString("uploadPort", uploadPortStr.getText().toString());

                EditText downloadPortStr = (EditText) findViewById(R.id.download_port);
                editor.putString("downloadPort", downloadPortStr.getText().toString());

                EditText interactivePortStr = (EditText) findViewById(R.id.interactive_port);
                editor.putString("interactivePort", interactivePortStr.getText().toString());

                EditText burstSizeStr = (EditText) findViewById(R.id.burst_size);
                editor.putString("burstSize", burstSizeStr.getText().toString());

                EditText burstFactorStr = (EditText) findViewById(R.id.burst_factor);
                editor.putString("burstFactor", burstFactorStr.getText().toString());

                EditText intervalSizeStr = (EditText) findViewById(R.id.interval_size);
                editor.putString("intervalSize",intervalSizeStr.getText().toString());

                EditText intervalTimeStr = (EditText) findViewById(R.id.interval_time);
                editor.putString("intervalTime", intervalTimeStr.getText().toString());

                EditText minSpeedStr = (EditText) findViewById(R.id.min_speed);
                editor.putString("minSpeed", minSpeedStr.getText().toString());

                EditText maxSpeedStr = (EditText) findViewById(R.id.max_speed);
                editor.putString("maxSpeed", maxSpeedStr.getText().toString());

                EditText startSpeedStr = (EditText) findViewById(R.id.start_speed);
                editor.putString("startSpeed", startSpeedStr.getText().toString());

                EditText gracePeriodStr = (EditText) findViewById(R.id.grace_period);
                editor.putString("gracePeriod", gracePeriodStr.getText().toString());

                EditText instantBurstStr = (EditText) findViewById(R.id.instant_burst);
                editor.putString("instantBurst", instantBurstStr.getText().toString());

                editor.commit();

                Intent mainActivityIntent = new Intent(ConfigurationActivity.this, MainActivity.class);
                startActivity(mainActivityIntent);
            }
        });
    }
}
