
//220101039 ESRA DEMİRTAŞ
//YÜKLEDİĞİNİZ QUESTİON.CSV DOSYASINDAN ID BİLGİSİNİ KODDA KULLANMADIĞIM İÇİN
// SORULARIN BİR KISMINI ID SİZ ŞEKİLDE DÜZENLEYİP YENİ SORU DOSYASIYLA  KODUMU ÇALIŞTIRDIM (kodu test etmek için )
// (level no- soru-şıklar-doğru cevap şeklinde düzenledim)

//Ödevde zaman zaman kafam çok karıştı varsa gözümden kaçan eksiklikler kusura bakmayın .



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 50
#define MAX_QUESTION_LENGTH 200
#define MAX_CHOICES 4
#define MAX_CONTESTANTS 100
#define MAX_QUESTIONS 100

// Soru struct yapısı
struct Question {
    int level_no;
    char question[MAX_QUESTION_LENGTH];
    char choices[MAX_CHOICES][50];
    char correctChoice;
};

// Oyuncu yapısı
struct Player {
    char name[MAX_NAME_LENGTH];
    int earnings; //kazanc
};

// Veri yapısı
struct Data {
    int question_count;
    struct Question questions[MAX_QUESTIONS];
};
//Gerekli fonksiyonlar
void welcome();
void showReport();
void saveReport(struct Player player);
void playGame(struct Data *data, struct Player players[], int *player_count);
void initializeData(struct Data *data);
void readData(struct Data *data);
void askQuestion(struct Question question);
void fiftyFifty(struct Question *question);
int doubleAnswer(struct Question *question);
int getRandomQuestionIndex(int maxIndex, int usedIndices[], int usedCount);
int calculateEarnings(int);

int main() {
    char choice;
    struct Data data;
    struct Player players[MAX_CONTESTANTS];
    int player_count = 0;

    welcome();

    do {
        printf("\n\n");
        printf("1. Yeni oyuna basla\n");
        printf("2. Raporu goster\n");
        printf("3. Oyundan cik\n");
        printf("   Seciminizi yapin: ");
        scanf(" %c", &choice);

          // Kullanıcının seçimine göre işlemlerin yapılması
        switch(choice) {
            case '1':
                playGame(&data, players, &player_count);
                break;
            case '2':
                showReport();
                break;
            case '3':
                printf("Oyundan ciktiniz.\n");
                break;

            default:
                printf("Gecersiz secim! Lutfen tekrar deneyin.\n");
        }
    } while(choice != '3'); //Oyundan çıkmadıkça tekrarlar

    return 0;
}

void welcome() {
    printf("KIM MILYONER OLMAK ISTER'E HOS GELDINIZ!\n");
    printf("BEN ESRA HAZIRSANIZ BASLAYALIM!\n");
}

// Raporu göster
void showReport() {
    FILE *file;
    char name[MAX_NAME_LENGTH];
    int earnings;

    file = fopen("report.txt", "r");

    if (file == NULL) {
        printf("Rapor bulunamadi.\n");
        return;
    }
    //oyuncular ve skorlar
    printf("\n----- Rapor -----\n");
    printf("Isim\tKazanc\n");
    while (fscanf(file, "%s %d", name, &earnings) == 2) {
        printf("%s\t%d\n", name, earnings);
    }

    fclose(file);
}

// Raporu kaydet
void saveReport(struct Player player) {
    FILE *file;
    file = fopen("report.txt", "a");

    if (file == NULL) {
        printf("Rapor dosyasi olusturulamadi.\n");
        return;
    }

    fprintf(file, "%s %d\n", player.name, player.earnings);

    fclose(file);
}

// Oyunu baslat
void playGame(struct Data *data, struct Player players[], int *player_count) {
    struct Player player;
    int question_index;
    char answer;
    int usedIndices[MAX_QUESTIONS] = {0};
    int usedCount = 0;
    int correct_answers = 0; // Doğru cevap sayısını tutmak için bir değişken

    printf("Oyuncunun ismini girin: ");
    scanf("%s", player.name);
    printf("\nBOL SANSLAR %s!\n", player.name);
    player.earnings = 0;

    srand(time(NULL));

    initializeData(data);
    readData(data);

    if (data->question_count == 0) {
        printf("Yuklenecek soru bulunamadi!\n");
        return;
    }

    // Oyun döngüsü
    for (int i = 0; i < 12; i++) {
        question_index = getRandomQuestionIndex(data->question_count, usedIndices, usedCount);
        usedIndices[usedCount++] = question_index;
        askQuestion(data->questions[question_index]);

        // Fifty-fifty özelliğini kullanmak isteyip istemediğini sor
        printf("Fifty-Fifty kullanmak ister misiniz? (E/H): ");
        char fifty_fifty_choice;
        scanf(" %c", &fifty_fifty_choice);
        int use_fifty_fifty = (fifty_fifty_choice == 'E' || fifty_fifty_choice == 'e');

        // Çift cevap hakkı özelliğini kullanmak isteyip istemediğini sor
        printf("Cift Cevap Hakki kullanmak ister misiniz? (E/H): ");
        char double_answer_choice;
        scanf(" %c", &double_answer_choice);
        int use_double_answer = (double_answer_choice == 'E' || double_answer_choice == 'e');


        // Fifty-fifty özelliğini kullan
        if (use_fifty_fifty) {
            fiftyFifty(&data->questions[question_index]);
        }

        // Çift cevap hakkı özelliğini kullan
        if (use_double_answer) {
            if (doubleAnswer(&data->questions[question_index])) {
                // Eğer çift cevap hakkı doğru kullanıldıysa, bir sonraki döngü adımını atlama
                continue;
            } else {
                // Eğer yanlış kullanıldıysa, oyunu bitir
                break;
            }
        }


        printf("Cevabinizi girin (A/B/C/D): ");
        scanf(" %c", &answer);

        if (answer == data->questions[question_index].correctChoice) {
            printf("Dogru!\n");

            // Doğru cevap sayısını artır
            correct_answers++;

            // Kazanç miktarını güncelle
            player.earnings = calculateEarnings(correct_answers);

            // Ödül kazanma durumunu kontrol et
            if (correct_answers == 2)
            {
                printf("\nTebrikler ilk baraji gectiniz\n");

            }
            if(correct_answers==7)
            {
                printf("Tebrikler ikinci baraji gectiniz");
            }
            if(correct_answers==12)
            {
                printf("BUYUK ODUL 1000000 SİZİN OLDU!");
            }

            }
          else {
            printf("Yanlis cevap! Oyun bitti.\n");
            if (i >= 7) {
                player.earnings = 50000;
            } else if (i >=2 ) {
                player.earnings = 5000;
            }
            break;
        }
}

    printf("Kazandiginiz toplam para: %d TL\n", player.earnings);

    players[*player_count] = player;
    (*player_count)++;

    saveReport(player);  // Oyuncu bilgilerini rapora kaydet

}

// Doğru cevap sayısına göre kazanç miktarını hesapla
int calculateEarnings(int correct_answers) {
    switch(correct_answers) {
        case 1:
            return 2000;
        case 2:
            return 5000;
        case 3:
            return 7500;
        case 4:
            return 10000;
        case 5:
            return 20000;
        case 6:
            return 30000;
        case 7:
            return 50000;
        case 8:
            return 100000;
        case 9:
            return 200000;
        case 10:
            return 300000;
        case 11:
            return 500000;
        case 12:
            return 1000000;
        default:
            return 0; // Hata durumu
    }
}

// Veri yapisini baslat
void initializeData(struct Data *data) {
    data->question_count = 0;
}

// Veriyi oku
void readData(struct Data *data) {
    FILE *file;
    char line[1024];
    char *token;

    file = fopen("SORULAR.csv", "r");
    if (file == NULL) {
        printf("Veri dosyasi bulunamadi.\n");
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        token = strtok(line, ",");
        data->questions[data->question_count].level_no = atoi(token);

        token = strtok(NULL, ",");
        strcpy(data->questions[data->question_count].question, token);

        for (int i = 0; i < MAX_CHOICES; i++) {
            token = strtok(NULL, ",");
            strcpy(data->questions[data->question_count].choices[i], token);
        }

        token = strtok(NULL, ",");
        data->questions[data->question_count].correctChoice = token[0];

        data->question_count++;
    }

    fclose(file);
}

// Soruyu ekrana yazdıran fonksiyon
void askQuestion(struct Question question) {

    printf("\nSoru: %s\n", question.question);
    for (int i = 0; i < MAX_CHOICES; i++) {
        printf("%c) %s\n", 'A' + i, question.choices[i]);
    }
}

// Yuzde elli joker
void fiftyFifty(struct Question *question) {
    int count = 0;
    if (question == NULL) {
        printf("Hata: Soru bulunamadi.\n");
        return;
    }
    for (int i = 0; i < MAX_CHOICES; i++) {
        if (count < 2 && ('A' + i) != question->correctChoice) {
            printf("Secenek %c elendi.\n", 'A' + i);
            count++;
        }
    }
}

// Cift cevap hakki
int doubleAnswer(struct Question *question) {
    char answer;
    printf("Iki kez cevap verebilirsiniz. Ilk cevabinizi girin: ");
    scanf(" %c", &answer);
    if (answer == question->correctChoice) {
        printf("Dogru!\n");
        return 1;
    } else {
        printf("Yanlis cevap!\n");
        printf("Ikinci kez cevap verebilirsiniz: ");
        scanf(" %c", &answer);
        if (answer == question->correctChoice) {
            printf("Dogru!\n");
            return 1;
        } else {
            printf("Yanlis cevap! Oyun bitti.\n");
            return 0;
        }
    }
}

// Rastgele bir soru indexi al
int getRandomQuestionIndex(int maxIndex, int usedIndices[], int usedCount) {
    int index;
    int unique;
    do {
        unique = 1;
        index = rand() % maxIndex;
        for (int i = 0; i < usedCount; i++) {
            if (usedIndices[i] == index) {
                unique = 0;
                break;
            }
        }
    } while (!unique);
    return index;
}


