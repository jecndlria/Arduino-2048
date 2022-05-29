void initializeSevenSegmentDisplay();   // DEFINED IN shiftregister_4digit7seg.ino
void sevenSegmentTimerTest();           // DEFINED IN shiftregister_4digit7seg.ino
void initializeScreen();
void testScreen();

void setup()
{
    //initializeScreen();
    initializeSevenSegmentDisplay();   
    initializeScreen();
    testScreen();
}
void loop()
{
    sevenSegmentTimerTest();
    //testScreen();
}