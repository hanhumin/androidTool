// IBookManager.aidl
package com.example.txl.tool.inter.process.communication;

// Declare any non-default types here with import statements

import com.example.txl.tool.inter.process.communication.Book;
import com.example.txl.tool.inter.process.communication.IOnBookArrivedListener;
interface IBookManager {
    /**
     * Demonstrates some basic types that you can use as parameters
     * and return values in AIDL.
     */
    List<Book> getBookList();
    void addBook(in Book book);
    void registerBookArrivedListener(in IOnBookArrivedListener listenser);
    void unRegisterBookArrivedListener(in IOnBookArrivedListener listenser);
}
