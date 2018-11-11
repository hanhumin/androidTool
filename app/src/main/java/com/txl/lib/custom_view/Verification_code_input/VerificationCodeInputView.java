package com.txl.lib.custom_view.Verification_code_input;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.text.Editable;
import android.text.InputFilter;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.example.txl.tool.R;

/**
 * Copyright (c) 2018, 唐小陆 All rights reserved.
 * author：txl
 * date：2018/11/11
 * description：验证码输入
 */
public class VerificationCodeInputView extends RelativeLayout {
    private RelativeLayout rootView;
    private final Context mContext;
    private LinearLayout containerLinear;
    private EditText verificationCodeInputEt;
    /**
     * 验证码数量
     * */
    private int mEtNumber;
    /**
     * 输入框宽度
     * */
    private int mEtWidth;
    /**
     * 输入框之间的分割线
     * */
    private Drawable mEtDividerDrawable;
    /**
     * 输入框文字大小
     * */
    private int mEtTextSize;
    /**
     * 输入框文字的颜色
     * */
    private int mEtTextColor;
    /**
     * 输入框边框的颜色
     * */
    private Drawable mEtBackgroundDrawableFocus;
    private Drawable mEtBackgroundDrawableNormal;

    /**
     * 输入框之间的间距
     * */
    private int mEtSpace;

    private TextView[] mTextViews;
    private Drawable focusDrawable;
    private Drawable normalDrawable;


    public VerificationCodeInputView(Context context) {
        this( context ,null);
    }

    public VerificationCodeInputView(Context context, AttributeSet attrs) {
        this( context, attrs , 0);
    }

    public VerificationCodeInputView(Context context, AttributeSet attrs, int defStyleAttr) {
        super( context, attrs, defStyleAttr );
        mContext = context;
        init(context,attrs,defStyleAttr);
    }

    private void init(Context context, AttributeSet attrs, int defStyleAttr) {
        rootView = (RelativeLayout) LayoutInflater.from(context).inflate( R.layout.verification_code_input_view, this,false);
        containerLinear =  rootView.findViewById(R.id.verification_code_container);
        verificationCodeInputEt = rootView.findViewById(R.id.et_verification_code_input);

        TypedArray typedArray = context.obtainStyledAttributes(attrs, R.styleable.VerificationCodeInputView, defStyleAttr, 0);
        mEtNumber = typedArray.getInteger(R.styleable.VerificationCodeInputView_icv_et_number, 1);
        mEtSpace = typedArray.getInteger(R.styleable.VerificationCodeInputView_icv_et_spacing, 5 );
        mEtWidth = typedArray.getDimensionPixelSize(R.styleable.VerificationCodeInputView_icv_et_width, 42);
        mEtDividerDrawable = typedArray.getDrawable(R.styleable.VerificationCodeInputView_icv_et_divider_drawable);
        mEtTextSize = typedArray.getDimensionPixelSize(R.styleable.VerificationCodeInputView_icv_et_text_size, 16);
        mEtTextColor = typedArray.getColor(R.styleable.VerificationCodeInputView_icv_et_text_color, Color.WHITE);
        mEtBackgroundDrawableFocus = typedArray.getDrawable(R.styleable.VerificationCodeInputView_icv_et_bg_focus);
        mEtBackgroundDrawableNormal = typedArray.getDrawable(R.styleable.VerificationCodeInputView_icv_et_bg_normal);
        Drawable defaultFocusDrawable = context.getResources().getDrawable( R.drawable.under_line_focus );
        Drawable defaultNormalDrawable = context.getResources().getDrawable( R.drawable.under_line_normal );
        focusDrawable = mEtBackgroundDrawableFocus!=null ? mEtBackgroundDrawableFocus: defaultFocusDrawable;
        normalDrawable = mEtBackgroundDrawableNormal!=null ? mEtBackgroundDrawableNormal: defaultNormalDrawable;
        typedArray.recycle();
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();
        initTextViews(mContext, mEtNumber, mEtWidth,mEtDividerDrawable,mEtTextSize,mEtTextColor);
        setListener();
    }

    private void initTextViews(Context context, int etNumber, int etWidth, Drawable etDividerDrawable, float etTextSize, int etTextColor) {
        //将光标隐藏
        verificationCodeInputEt.setCursorVisible(false);
        //最大输入长度
        verificationCodeInputEt.setFilters(new InputFilter[]{new InputFilter.LengthFilter(etNumber)});
        // 设置分割线的宽度
        if (etDividerDrawable != null) {
            etDividerDrawable.setBounds(0, 0, etDividerDrawable.getMinimumWidth(), etDividerDrawable.getMinimumHeight());
            containerLinear.setDividerDrawable(etDividerDrawable);
        }
        verificationCodeInputEt.setTextSize(etTextSize);
        verificationCodeInputEt.setLongClickable( false );
        mTextViews = new TextView[etNumber];
        for (int i = 0; i < etNumber; i++) {
            TextView textView = new TextView(context);
            textView.setTextSize(etTextSize);
            textView.setTextColor(etTextColor);
            if (i == 0) {
                textView.setBackground(mEtBackgroundDrawableFocus);
            } else {
                textView.setBackground(mEtBackgroundDrawableNormal);
            }
            textView.setGravity( Gravity.CENTER);

            textView.setFocusable(false);

            mTextViews[i] = textView;
            LinearLayout.LayoutParams layoutParams =new LinearLayout.LayoutParams( etWidth, ViewGroup.LayoutParams.WRAP_CONTENT );
            if (i != 0 && i < etNumber) {
                layoutParams.leftMargin = mEtSpace;
            }
            containerLinear.addView( textView ,layoutParams);
        }
        addView( rootView );
    }

    private void setListener() {
        // 监听输入内容
        verificationCodeInputEt.addTextChangedListener(new TextWatcher() {

            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
            }

            @Override
            public void afterTextChanged(Editable editable) {
                String inputStr = editable.toString();
                if ( !inputStr.equals("")) {
                    setText(inputStr);
                    verificationCodeInputEt.setText("");
                }
            }
        });

        // 监听删除按键
        verificationCodeInputEt.setOnKeyListener(new OnKeyListener() {
            @Override
            public boolean onKey(View v, int keyCode, KeyEvent event) {
                if (keyCode == KeyEvent.KEYCODE_DEL && event.getAction() == KeyEvent.ACTION_DOWN) {
                    onKeyDelete();
                    return true;
                }
                return false;
            }
        });
    }

    private void setText(String inputStr) {

        for (int i = 0; i < mTextViews.length; i++) {
            TextView tv = mTextViews[i];
            if (tv.getText().toString().trim().equals("")) {
                tv.setText(inputStr);
                // 添加输入完成的监听
                if (inputCompleteListener != null) {
                    inputCompleteListener.inputComplete();
                    if(i == mTextViews.length-1){
                        inputCompleteListener.inputCompleteAll();
                    }
                }
                tv.setBackground(normalDrawable);
                if (i < mEtNumber - 1) {
                    mTextViews[i + 1].setBackground(focusDrawable);
                }
                break;
            }
        }
    }

    public void onKeyDelete() {
        for (int i = mTextViews.length - 1; i >= 0; i--) {
            TextView tv = mTextViews[i];
            if (!tv.getText().toString().trim().equals("")) {
                tv.setText("");
                // 添加删除完成监听
                if (inputCompleteListener != null) {
                    inputCompleteListener.deleteContent();
                }
                tv.setBackground(focusDrawable);
                if (i < mEtNumber - 1) {
                    mTextViews[i + 1].setBackground(normalDrawable);
                }
                break;
            }
        }
    }

    /**
     * 获取输入文本
     */
    public String getTextContent() {
        StringBuffer buffer = new StringBuffer();
        for (TextView tv : mTextViews) {
            buffer.append(tv.getText().toString().trim());
        }
        return buffer.toString();
    }

    /**
     * 删除所有内容
     */
    public void clearAllText() {
        for (int i = 0; i < mTextViews.length; i++) {
            if (i == 0) {
                mTextViews[i].setBackground(mEtBackgroundDrawableFocus);
            } else {
                mTextViews[i].setBackground(mEtBackgroundDrawableNormal);
            }
            mTextViews[i].setText("");
        }
    }

    // 输入完成 和 删除成功 的监听
    private InputCompleteListener inputCompleteListener;

    public void setInputCompleteListener(InputCompleteListener inputCompleteListener) {
        this.inputCompleteListener = inputCompleteListener;
    }

    public interface InputCompleteListener {
        void inputComplete();

        void deleteContent();

        void inputCompleteAll();
    }
}
