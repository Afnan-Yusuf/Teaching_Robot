import speech_recognition as sr
import pyttsx3
import google.generativeai as genai
import os
from dotenv import load_dotenv
import tkinter as tk
from tkinter import scrolledtext, font
import threading
import time
import random
import sys

class FridayAITeacher:
    def _init_(self):
        # Load environment variables
        load_dotenv()

        # Configure Google Gemini AI
        genai.configure(api_key="AIzaSyAjCm594tU2HW5icaVmqbciFgNhmtAaFfg")
        self.model = genai.GenerativeModel('gemini-pro')

        # Initialize speech recognition and text-to-speech
        self.recognizer = sr.Recognizer()
        self.engine = pyttsx3.init()

        # Set voice to sweet
        voices = self.engine.getProperty('voices')
        self.engine.setProperty('voice', voices[1].id)  # Use the second available voice (usually female)
        self.engine.setProperty('rate', 150)  # Maintain the speech rate
        self.engine.setProperty('pitch', 200)  # Increase the pitch for a sweeter sound

        # Initialize conversation history
        self.conversation_history = []

        # Create the main window
        self.root = tk.Tk()
        self.root.title("FRIDAY AI English Teacher")
        self.root.geometry("800x480")
        self.root.configure(bg='black')

        # Create custom font
        self.custom_font = font.Font(family="Arial", size=12, weight="bold")

        # Create and pack the teacher animation label
        self.teacher_label = tk.Label(self.root, font=("Courier", 18), fg="white", bg="black")
        self.teacher_label.pack(pady=10)

        # Create and pack the display widget
        self.display = scrolledtext.ScrolledText(self.root, wrap=tk.WORD, width=80, height=20, bg="black", fg="white", font=self.custom_font)
        self.display.pack(expand=True, fill='both', padx=20, pady=10)
        self.display.configure(state='disabled')

        # Configure text colors
        self.display.tag_configure("yellow", foreground="yellow", justify='center')
        self.display.tag_configure("cyan", foreground="cyan", justify='center')
        self.display.tag_configure("lime", foreground="lime", justify='center')
        self.display.tag_configure("red", foreground="red", justify='center')

        # Flag to control teacher's active state
        self.teacher_active = True

        # List of alternative prompts
        self.listening_prompts = [
            "What would you like to say?".center(80),
            "I'm here to listen.".center(80)
        ]
        self.retry_prompts = [
            "Could you please repeat that?".center(80),
            "I'm having trouble hearing. One more time?".center(80),
            "I missed that. Can you say it again?".center(80)
        ]

        # Set up automatic restart
        self.root.protocol("WM_DELETE_WINDOW", self.restart_app)

    def listen(self):
        with sr.Microphone() as source:
            prompt = random.choice(self.listening_prompts + self.retry_prompts)
            self.clear_display()
            self.update_display(prompt, "yellow")
            self.recognizer.adjust_for_ambient_noise(source, duration=1)
            self.recognizer.dynamic_energy_threshold = True
            self.recognizer.pause_threshold = 0.5
            try:
                audio = self.recognizer.listen(source, timeout=6, phrase_time_limit=10)
                text = self.recognizer.recognize_google(audio)
                self.update_display(f"You said: {text}".center(80), "cyan")
                return text
            except (sr.WaitTimeoutError, sr.UnknownValueError, sr.RequestError):
                return None

    def speak(self, text):
        self.update_display(f"FRIDAY: {text}".center(80), "lime")
        self.engine.say(text)
        self.engine.runAndWait()

    def get_ai_response(self, prompt):
        try:
            response = self.model.generate_content(prompt)
            return response.text
        except Exception as e:
            self.update_display(f"Error getting AI response: {str(e)}".center(80), "red")
            return "I'm sorry, I'm having trouble thinking right now. Can you please try again?"

    def update_display(self, text, color="white"):
        self.display.configure(state='normal')
        self.display.insert(tk.END, text + "\n", color)
        self.display.see(tk.END)
        self.display.configure(state='disabled')

    def clear_display(self):
        self.display.configure(state='normal')
        self.display.delete('1.0', tk.END)
        self.display.configure(state='disabled')

    def english_teacher(self):
        self.speak("Hello! I'm your AI English teacher. My name is FRIDAY. How can I help you today?")
        
        while True:
            user_input = self.listen()
            if user_input:
                if "quit" in user_input.lower() or "exit" in user_input.lower() or "good bye" in user_input.lower():
                    self.speak("Goodbye! I'll be quiet now. Call my name or say 'teacher' when you need me again.")
                    self.teacher_active = False
                    continue
                
                if not self.teacher_active and ("friday" in user_input.lower() or "teacher" in user_input.lower()):
                    self.speak("I'm back! How can I help you?")
                    self.teacher_active = True
                    continue
                
                if not self.teacher_active:
                    continue
                
                self.conversation_history.append(f"Student: {user_input}")
                
                history_context = "\n".join(self.conversation_history[-5:])
                prompt = f"""
                You are FRIDAY, an AI English teacher for students under first standard. Here's the recent conversation history:

                {history_context}

                Based on this context, provide a helpful, concise response to teach or correct their English.
                If they asked a question, answer it.
                If there were grammatical errors, politely point them out and provide the correct form.
                If they used a word incorrectly, explain the proper usage and give response to asked question.
                Keep your response short, simple, and focused on one or two key points, appropriate for a first-grade student.
                """
                
                ai_response = self.get_ai_response(prompt)
                self.speak(ai_response)
                
                self.conversation_history.append(f"FRIDAY: {ai_response}")

    def restart_app(self):
        self.root.destroy()
        python = sys.executable
        os.execl(python, python, *sys.argv)

    def run(self):
        # Start the English teacher in a separate thread
        threading.Thread(target=self.english_teacher, daemon=True).start()

        # Start the Tkinter event loop
        self.root.mainloop()

if _name_ == "_main_":
    app = FridayAITeacher()
    app.run()