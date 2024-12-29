Proof of concept. ESP32 project that uses our student SmartTags for a simple sign-in system for WW Robotics.

Current Register Process (Using our student SmartTags) : 
Step 1: The user brings their card to the reader.
Step 2: The system reads the unique UID from the card.
Step 3: The user is prompted that their card UID is successfully registered. (The registration info is now stored on a Google Sheet)
Step 4: The user can now use the card to mark attendance in the future

Attendance Process (for marking attendance):
Step 1: The user brings their card close to the reader.
Step 2: The system reads the UID of the card
Step 3: If the card is registered, the system checks the attendance status.
    -If new: It registers the user’s attendance (Signed in).
    -If already attended: Marks the Google Sheet as leaving (Signed out)
Step 4: The user can check the time and date of their attendance and how long they stayed for on the LCD display. 
