package com.example.txl.tool.bluetooth;

import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.example.txl.tool.R;
import com.example.txl.tool.base.BaseAdapter;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.util.ArrayList;

public class FoundDeviceAdapter extends BaseAdapter<BluetoothDevice> {
    public FoundDeviceAdapter(@NotNull Context context, @Nullable ArrayList<BluetoothDevice> list) {
        super(context, list);
    }

    public FoundDeviceAdapter(@NotNull Context context) {
        super(context);
    }

    @NonNull
    @Override
    public BaseViewHolder<BluetoothDevice> onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        return new BoundDeviceViewHolder( getLayoutInflater().inflate(R.layout.item_bluetooth_found,parent,false));
    }

    private static class BoundDeviceViewHolder extends  BaseViewHolder<BluetoothDevice>{
        TextView tvName = itemView.findViewById(R.id.tv_name);
        public BoundDeviceViewHolder(@NotNull View itemView) {
            super(itemView);
        }

        @Override
        public void onBindViewHolder(int position, BluetoothDevice data, @NotNull BaseAdapter<BluetoothDevice> adapter) {
            super.onBindViewHolder(position, data, adapter);
            tvName.setText(data.getName());
        }
    }
}
