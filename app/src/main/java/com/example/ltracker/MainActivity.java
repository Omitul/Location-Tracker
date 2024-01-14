package com.example.ltracker;
import android.app.Dialog;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
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
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.app.Dialog;
import android.widget.ImageView;


public class MainActivity extends AppCompatActivity {
    EditText etBusId;
    TextView tvLatitude, tvLongitude, tvmap;
    DatabaseReference reference;
    Dialog dialog;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        etBusId = findViewById(R.id.etbusid);
        tvLatitude = findViewById(R.id.tvLatitude);
        tvLongitude = findViewById(R.id.tvLongitude);
        tvmap = findViewById(R.id.tvmap);

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

        /// for the bus schedule
        dialog = new Dialog(this);
        findViewById(R.id.bus_schedule_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.d("MainActivity", "onClick method invoked");
                try {
                    dialog = new Dialog(MainActivity.this);
                    ImageView imageView = new ImageView(MainActivity.this);

                    imageView.setImageResource(R.drawable.bus_schedule);

                    dialog.setContentView(imageView);

                    dialog.show();
                } catch (Exception e) {
                    e.printStackTrace();
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
                       // Log.d("ReadData", "Data read successfully");
                        Toast.makeText(MainActivity.this, "Location  found!", Toast.LENGTH_LONG).show();
                        DataSnapshot busData = task.getResult();

                        // Retrieve latitude and longitude directly
                        String latitude = busData.child("Latitude").getValue(String.class);
                        String longitude = busData.child("Longitude").getValue(String.class);
                        String mapLink = "http://maps.google.com/maps?q=" + latitude + "," + longitude;
                        double Longitude = 0.0;
                        String modifiedString = longitude.substring(0, longitude.length() - 1);
                        Longitude = Double.valueOf((modifiedString));
                        double Latitude = 0.0;
                        modifiedString = latitude.substring(0, latitude.length() - 1);
                        Latitude = Double.valueOf((modifiedString));


                        // setting the values in the field in string format, because only strings are allowed to set
                        tvLatitude.setText(String.valueOf(Latitude));
                        tvLongitude.setText(String.valueOf(Longitude));
                        tvmap.setText("Google Map");
                        Intent intent = new Intent(Intent.ACTION_VIEW);
                        intent.setData(Uri.parse(mapLink));

                        tvmap.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                // Create an Intent with the ACTION_VIEW action
                                Intent intent = new Intent(Intent.ACTION_VIEW);

                                // Set the data of the Intent to the mapLink
                                intent.setData(Uri.parse(mapLink));

                                // Check if there is any application available to handle the Intent
                                if (intent.resolveActivity(getPackageManager()) != null) {
                                    // Start the activity if a suitable app is found
                                    startActivity(intent);
                                } else {
                                    // Handle the case where Google Maps is not installed
                                    // You can show a toast or a dialog prompting the user to install Google Maps
                                    Toast.makeText(MainActivity.this, "Google Maps is not installed", Toast.LENGTH_SHORT).show();
                                }
                            }
                        });
                    } else {
                       // Log.d("ReadData", "Data doesn't exist");
                        Toast.makeText(MainActivity.this, "Location is not  found!", Toast.LENGTH_LONG).show();
                    }
                } else {
                    Log.d("ReadData", "Error reading data: ", task.getException());
                    Toast.makeText(MainActivity.this, "Error reading data: " + task.getException(), Toast.LENGTH_LONG).show();
                }
            }
        });
    }
}
