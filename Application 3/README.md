## English Writing Test Program

This program is designed as an English writing test application. Its functionality is as follows:

1. **User Input**: 
   - The user enters the number of words, time (in minutes), and the location to save the output file (it is desktop by default).

2. **Test Environment**: 
   - The program opens a new window in full-screen mode, preventing the user from accessing other applications.

3. **Submission**:
   - The user can submit their writing either before or after the allotted time:
     1. If the user finishes the required word count within the time limit, the file is labeled as a "successful try".
     2. If the user does not finish, it will be labeled as a "failed try".
   - If time runs out while the writing window is still open, the program will automatically close. The test file will be saved in the specified location with either a "successful" or "failed" label based on the word count.

4. **File Encoding**:
   - The test file is encoded using UFI64 and Chinese characters to prevent tampering or copying.
   - Both the user and the examiner can decode the file to reveal the written content. The decoded file is automatically saved to the desktop path. 

5. **Exam Integrity**:
   - The encoding/decoding mechanism ensures that the test was completed within the application and submitted correctly.
   - Additional information such as the input word count, actual word count, input time, and actual time taken is also included in the text file.

6. **Result Check**:
   - The test-taker and the examiner can check their exam results within the application.
