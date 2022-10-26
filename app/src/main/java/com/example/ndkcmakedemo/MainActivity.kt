package com.example.ndkcmakedemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.example.ndkcmakedemo.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.stringText.text = StringBuilder("stringFromJNI: " + stringFromJNI())
        binding.intText.text = StringBuilder("intFromJNI: " + intFromJNI())
        binding.floatText.text = StringBuilder("floatFromJNI: " + floatFromJNI(18.3f,4.2f))
        binding.booleanText.text = StringBuilder("booleanFromJNI: " + booleanFromJNI())
    }

    /**
     * A native method that is implemented by the 'ndkcmakedemo' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun intFromJNI(): Int
    external fun floatFromJNI(num1 : Float, num2: Float): Float
    external fun booleanFromJNI(): Boolean


    companion object {
        // Used to load the 'ndkcmakedemo' library on application startup.
        init {
            System.loadLibrary("ndkcmakedemo")
            System.loadLibrary("jnitest")
        }
    }
}