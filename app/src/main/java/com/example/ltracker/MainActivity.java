package com.example.ltracker;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class MainActivity extends AppCompatActivity {
    EditText etBusId;
    TextView tvLatitude, tvLongitude;
    DatabaseReference reference;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        etBusId = findViewById(R.id.etbusid);
        tvLatitude = findViewById(R.id.tvLatitude);
        tvLongitude = findViewById(R.id.tvLongitude);

        findViewById(R.id.readdataBtn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String busLocation = etBusId.getText().toString().trim().toLowerCase(); // Convert to lowercase for consistency
                if (!busLocation.isEmpty()) {
                    readData(busLocation);
                } else {
                    System.out.println("HELLO PITTER");
                    Toast.makeText(MainActivity.this, "Please Enter The Bus Id!", Toast.LENGTH_LONG).show();
                }
            }

        });
    }

    private void readData(String busLocation) {
        reference = FirebaseDatabase.getInstance("https://ltracker-64a48-default-rtdb.asia-southeast1.firebasedatabase.app").getReference("Bus Location");

        reference.child(busLocation).get().addOnCompleteListener(new OnCompleteListener<DataSnapshot>() {
            @Override
            public void onComplete(@NonNull Task<DataSnapshot> task) {
                if (task.isSuccessful()) {
                    if (task.getResult().exists()) {
                        Log.d("ReadData", "Data read successfully");
                        DataSnapshot busData = task.getResult();

                        // Retrieve latitude and longitude directly
                        String latitude = busData.child("Latitude").getValue(String.class);
                        String longitude = busData.child("Longitude").getValue(String.class);
                        double Longitude = 0.0;
                        String modifiedString = longitude.substring(1, longitude.length() - 1);
                        Longitude = Double.valueOf((modifiedString));
                        double Latitude = 0.0;
                        modifiedString = latitude.substring(1, latitude.length() - 1);
                        Latitude = Double.valueOf((modifiedString));


                        // setting the values in the field in string format, because only strings are allowed to set
                        tvLatitude.setText(String.valueOf(Latitude));
                        tvLongitude.setText(String.valueOf(Longitude));

                        Toast.makeText(MainActivity.this, "Location Found!", Toast.LENGTH_LONG).show();
                    } else {
                        Log.d("ReadData", "Data doesn't exist");
                        Toast.makeText(MainActivity.this, "Location didn't found!", Toast.LENGTH_LONG).show();
                    }
                } else {
                    Log.d("ReadData", "Error reading data: ", task.getException());
                    Toast.makeText(MainActivity.this, "Error reading data: " + task.getException(), Toast.LENGTH_LONG).show();
                }
            }
        });
    }
}
