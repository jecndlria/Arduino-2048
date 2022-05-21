void initializeSevenSegmentDisplay();   // DEFINED IN shiftregister_4digit7seg.ino
void sevenSegmentTimerTest();           // DEFINED IN shiftregister_4digit7seg.ino

void setup()
{
    initializeSevenSegmentDisplay();    
}
void loop()
{
    sevenSegmentTimerTest();
}