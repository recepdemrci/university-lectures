package tr.edu.yildiz.recepdemirci;

import android.view.View;

public interface ClickListener {
    void onEditorSendClick(View view, int position);
    void onDeleteClick(View view, int position);
    void onSelectClick(View view, int position);
    void onSaveClick(View view, int position);
}
