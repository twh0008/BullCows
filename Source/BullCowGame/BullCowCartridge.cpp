// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Words, *WordListPath, [](const FString& Word) { return Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word); });
    Words = GetValidWords(Words);
    InitGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    ClearScreen();

    //if game over, clear screen and reset;
    if (bGameOver) {
        ClearScreen();
        InitGame();
        return;
    } 
    ProcessGuess(PlayerInput);

}
void UBullCowCartridge::InitGame() {
    //set lifes based on length of hidden word
    HiddenWord = Words[FMath::RandRange(0, Words.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Welcome to Bull Cows, the game with Bulls and Cows"));
    PrintLine(TEXT("The number of potential words is %i"), Words.Num());
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Hidden Word: %s"), *HiddenWord);
    PrintLine(TEXT("Type in your guess and press the enter key to continue..."));


    const TCHAR HW[] = TEXT("cakes");
}
void UBullCowCartridge::EndGame() {
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again..."));
}
void UBullCowCartridge::ProcessGuess(const FString& Guess) {

    if (Guess.Len() != HiddenWord.Len()) { //guess again
        PrintLine(TEXT("Your guess should be the length of %i. Guess again"), HiddenWord.Len());
        return;
    }
    if (!IsIsogram(Guess)) {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    if (Guess == HiddenWord) {
        PrintLine(TEXT("You win!"));
        EndGame();
        return;
    } 
        

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    
    PrintLine(TEXT("You lost! You have %i lives left."), --Lives);
        if (Lives == 0) {
            PrintLine(TEXT("You're out of lives. The hidden word was %s"), *HiddenWord);
            EndGame();
        }




        //check if input is isogram by array magic or hashset
        //while lives are > 0
        //bulls = matching letters
        //cows = non-matching letters
        //show lives left
        //if no lives show gameover and hidden word
        //prompt to play again
        //check player input
}

bool UBullCowCartridge::IsIsogram(const FString& Word) {
    
    for (int32 i = 0; i < Word.Len(); i++) {
        for (int32 k = i + 1; k < Word.Len(); k++) {
            if (Word[i] == Word[k]) {
                return false;
            }
        }
    }
    return true;
    
}
TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Words) const {
    
    TArray<FString> ValidWords;

    for (FString Word: Words) {
        if ((Word.Len() >= 4 && Word.Len() <= 8) && IsIsogram(Word)) {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
    
}
FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const  {
    
    FBullCowCount Count;
    
    for (int32 i = 0; i < Guess.Len(); i++) {
        if (Guess[i] == HiddenWord[i]) {
            Count.Bulls++;
            continue;
        } 

        for (int32 k = 0; k < HiddenWord.Len(); k++) {
            if (Guess[i] == HiddenWord[k]) {
                Count.Cows++;
                break;
            }
        }

    }

    return Count;

    //if index is same as index of Hidden word, then bullcount ++
    //is it a cow?
    
}