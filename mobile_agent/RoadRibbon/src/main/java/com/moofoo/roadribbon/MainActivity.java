package com.moofoo.roadribbon;

import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.location.LocationProvider;
import android.os.Environment;
import android.support.v7.app.ActionBarActivity;
import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Locale;

public class MainActivity extends ActionBarActivity {
    protected Location previousLocation = null;
    protected File outputFile = null;
    protected PrintWriter outputWriter = null;
    protected ArrayList<String> lines = new ArrayList<String>();

    protected void processLocation(Location location) {
        if ((this.previousLocation != null && location.distanceTo(this.previousLocation) > Math.pow(1.0, -3.0)) || (this.previousLocation == null)) {
            this.storeLocation(location);
        }

        this.previousLocation = location;
    }

    protected void flushLines() {
        try {
            if (this.outputFile == null) {
                Calendar cal = Calendar.getInstance();
                String filename = String.format("roadribbon_%s.txt",
                        new SimpleDateFormat("ddMMyyyy_hhmmss").format(cal.getTime())
                );

                File dir = new File(Environment.getExternalStorageDirectory(), "/RoadRibbon");
                dir.mkdirs();

                this.outputFile = new File(dir, filename);
            }

            FileOutputStream stream = new FileOutputStream(this.outputFile, true);
            this.outputWriter = new PrintWriter(stream);

            for (String line : this.lines) {
                this.outputWriter.print(line);
            }

            this.outputWriter.close();

            this.lines.clear();
        } catch (Exception e) {
            this.addTextMessage("----------------");
            this.addTextMessage(e.getMessage());

            for (StackTraceElement elt : e.getStackTrace()) {
                this.addTextMessage(elt.toString());
            }

            this.addTextMessage("----------------");

            Toast.makeText(getApplicationContext(), "Whoops... Exception...", Toast.LENGTH_LONG);

            return;
        }
    }

    protected void storeLocation(Location location) {
        try {
            String gprmcLine = this.formatGPRMCLine(location);
            String pgrmzLine = this.formatPGRMZLine(location);

            this.lines.add(gprmcLine);
            this.lines.add(pgrmzLine);

            this.addTextMessage(gprmcLine);
            this.addTextMessage(pgrmzLine);

            this.flushLines();
        } catch (Exception e) {
            this.addTextMessage("----------------");
            this.addTextMessage(e.getMessage());

            for (StackTraceElement elt : e.getStackTrace()) {
                this.addTextMessage(elt.toString());
            }

            this.addTextMessage("----------------");

            Toast.makeText(getApplicationContext(), "Whoops... Exception...", Toast.LENGTH_LONG);

            return;
        }
    }

    protected String formatDegrees(double lat) {
        long D = Math.round(lat);
        double m = (lat - D) * 60.0;
        DecimalFormatSymbols otherSymbols = new DecimalFormatSymbols(Locale.UK);
        //otherSymbols.setDecimalSeparator(',');
        //otherSymbols.setGroupingSeparator('.');

        String res = String.format("%02d%s",
                D,
                new DecimalFormat("##.##", otherSymbols).format(m)
        );

        return res;
    }

    protected String formatGPRMCLine(Location location) {
        // $GPRMC,hhmmss.ss,A,GGMM.MM,P,gggmm.mm,J,v.v,b.b,ddmmyy,x.x,n,m*hh
        // $GPRMC,120354.00,A,5514.32,N,10916.54,E,,,160114,,,A - position
        String lat = this.formatDegrees(location.getLatitude());
        String lng = this.formatDegrees(location.getLongitude());

        Calendar cal = Calendar.getInstance();
        String time = new SimpleDateFormat("HHmmss.00").format(cal.getTime());
        String date = new SimpleDateFormat("ddMMyy").format(cal.getTime());

        return String.format(
                "$GPRMC,%s,A,%s,N,%s,E,,,%s,,,A\r\n",
                time,
                lat,
                lng,
                date
        );
    }

    protected String formatPGRMZLine(Location location) {
        // $PGRMZ,93,m,3 - altitude
        long alt = Math.round(location.getAltitude());

        return String.format(
                "$PGRMZ,%02d,m,3\r\n",
                alt
        );
    }

    protected void setTextMessage(String text) {
        TextView myTextView = (TextView) findViewById(R.id.my_textview);

        myTextView.setText(text);
    }

    protected void addTextMessage(String text) {
        TextView myTextView = (TextView) findViewById(R.id.my_textview);

        myTextView.append(text + "\n");
    }

    protected void initializeLocationService() {
        LocationManager locationManager = (LocationManager) this.getSystemService(Context.LOCATION_SERVICE);

        LocationListener locationListener = new LocationListener() {
            public void onLocationChanged(Location location) {
                MainActivity.this.processLocation(location);
            }

            public void onStatusChanged(String provider, int status, Bundle extras) {
                MainActivity.this.addTextMessage((status != LocationProvider.AVAILABLE) ? "GPS UNAVAILABLE" : "GPS READY");
                String statusMsg = (status != LocationProvider.AVAILABLE) ? "UNAVAILABLE" : "ok";
                Toast.makeText(getApplicationContext(), statusMsg, Toast.LENGTH_SHORT).show();
            }

            public void onProviderEnabled(String provider) {
                Toast.makeText(getApplicationContext(), "Provider Enabled", Toast.LENGTH_SHORT).show();
            }

            public void onProviderDisabled(String provider) {
                Toast.makeText(getApplicationContext(), "Provider Disabled", Toast.LENGTH_SHORT).show();
            }
        };

        locationManager.requestLocationUpdates(
                LocationManager.GPS_PROVIDER,
                0, 0, locationListener);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        if (savedInstanceState == null) {
            getSupportFragmentManager().beginTransaction()
                    .add(R.id.container, new PlaceholderFragment())
                    .commit();
        }

        initializeLocationService();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        switch (item.getItemId()) {
            case R.id.action_settings:
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {
        public PlaceholderFragment() {
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                Bundle savedInstanceState) {
            View rootView = inflater.inflate(R.layout.fragment_main, container, false);
            TextView textView = (TextView) rootView.findViewById(R.id.my_textview);
            textView.setMovementMethod(new ScrollingMovementMethod());
            return rootView;
        }
    }
}
