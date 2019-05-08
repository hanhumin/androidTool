package com.example.txl.tool.design.pattern.state;

public class Main {
    public static void main(String[] args){
        UserStateContext userStateContext = UserStateContext.getUserStateContext();
        userStateContext.getmState().comment();
        userStateContext.setmState(new LoginState());
        userStateContext.getmState().comment();
    }
}
