package com.league.king.openglsample;

import android.Manifest;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.league.king.openglsample.util.PermissionUtil;

public class MainActivity extends AppCompatActivity {

    private String[] OPERA;

    private ListView    lv_opera;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        OPERA = getResources().getStringArray(R.array.operas);
        setContentView(R.layout.activity_main);

        lv_opera = (ListView) findViewById(R.id.main_lv_opera);
        ArrayAdapter<String> arrayAdapter =
                new ArrayAdapter<String>(this,
                        R.layout.activity_main_cell,
                        R.id.main_lv_opera_cell_tv_title,
                        OPERA);
        lv_opera.setAdapter(arrayAdapter);

        lv_opera.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, final int position, long id) {

                if (position == GLViewActivity.RENDERER_TEXTURE) {

                    boolean hasPermission = PermissionUtil.hasPermission(MainActivity.this, Manifest.permission.READ_EXTERNAL_STORAGE);
                    if (!hasPermission) {

                        PermissionUtil.needPermission(MainActivity.this, Manifest.permission.READ_EXTERNAL_STORAGE, new PermissionUtil.OnPermissionActionListener() {
                            @Override
                            public void onDenyPermissions(String[] permission, Boolean[] canRequestAgain) {
                                if (permission != null && permission.length > 0)
                                    Toast.makeText(MainActivity.this, "缺少权限", Toast.LENGTH_SHORT).show();
                            }

                            @Override
                            public void onAcceptPermissions(String permission) {

                                Intent intent = new Intent(MainActivity.this, GLViewActivity.class);
                                intent.putExtra(GLViewActivity.EXTRA_WHICH_RENDERER, position);
                                startActivity(intent);
                            }
                        });
                        return;
                    }
                }
                Intent intent = new Intent(MainActivity.this, GLViewActivity.class);
                intent.putExtra(GLViewActivity.EXTRA_WHICH_RENDERER, position);
                startActivity(intent);

            }
        });
    }
}
