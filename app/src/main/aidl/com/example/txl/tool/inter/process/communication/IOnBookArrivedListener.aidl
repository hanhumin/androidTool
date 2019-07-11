// IOnBookArrivedListener.aidl
package com.example.txl.tool.inter.process.communication;

// Declare any non-default types here with import statements
import com.example.txl.tool.inter.process.communication.Book;
interface IOnBookArrivedListener {
    void onNewBookArrived(in Book book);
}
