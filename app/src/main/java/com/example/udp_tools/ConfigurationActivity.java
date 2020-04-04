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
    private SharedPreferences pref;
    private Editor editor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_configuration);

        Button saveButton = findViewById(R.id.config_save_button);

        this.pref = getSharedPreferences("cellular-measurement", MODE_PRIVATE);
        this.editor = this.pref.edit();

        // go to InteractiveActivity when interactive button is clicked
        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // save values to shared preferences
                EditText interactiveName = findViewById(R.id.interactive_name);
                editor.putString("interactive_name", interactiveName.getText().toString());
                editor.commit();

                Intent mainActivityIntent = new Intent(ConfigurationActivity.this, MainActivity.class);
                startActivity(mainActivityIntent);
            }
        });
    }
}
