#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LENGTH_OF_NAME  50
#define Total_SLOT           5

typedef unsigned char      u8;
typedef unsigned short int u16;
typedef unsigned long int  u32;
typedef signed char        s8;
typedef signed short int   s16;
typedef signed long int    s32;
typedef float              f32;
typedef double             f64;
typedef long double        f128;

typedef struct data_patient {
    u16 ID;
    u8 name[MAX_LENGTH_OF_NAME];
    u8 age;
    u8 gender;
    struct data_patient *next;
} patient;

patient *head = NULL;
u16 slot_reservation[Total_SLOT] = {0};

/* Function Prototypes */
u8 admin_login(void);
patient *find_patient(u16 id);
u16 read_valid_u16(const char *prompt);
u8 read_valid_u8(const char *prompt);
void add_patient(void);
void edit_patient(void);
void delete_patient(void);
void reserve_slot(void);
void cancel_reservation(void);
void view_patient_record(void);
void view_reservation(void);
void flush_input(void);
void free_all_patients(void);

int main(void) {
    u8 mode_choice;
    u8 admin_mode, user_mode;

    while (1) {
        printf("\n====================================\n");
        printf("      Clinic Management System      \n");
        printf("====================================\n");
        printf("1. Admin Mode\n");
        printf("2. User Mode\n");
        printf("3. Exit System\n");
        printf("====================================\n");

        mode_choice = read_valid_u8("Choose Mode: ");

        if (mode_choice == 1) {
            // Admin Mode
            if (admin_login()) {
                while (1) {
                    printf("\n--- Admin Menu ---\n");
                    printf("1. Add new patient record\n");
                    printf("2. Edit patient record\n");
                    printf("3. Reserve a slot\n");
                    printf("4. Cancel reservation\n");
                    printf("5. Delete patient record\n");
                    printf("6. Logout to Main Menu\n");

                    admin_mode = read_valid_u8("Choice: ");

                    if (admin_mode == 1) add_patient();
                    else if (admin_mode == 2) edit_patient();
                    else if (admin_mode == 3) reserve_slot();
                    else if (admin_mode == 4) cancel_reservation();
                    else if (admin_mode == 5) delete_patient();
                    else if (admin_mode == 6) break; // Return to Main Menu
                    else printf("Invalid choice! Please try again.\n");
                }
            } else {
                break; // Exit system after 3 failed password attempts
            }
        } 
        else if (mode_choice == 2) {
            // User Mode
            while (1) {
                printf("\n--- User Menu ---\n");
                printf("1. View patient record\n");
                printf("2. View today's reservations\n");
                printf("3. Logout to Main Menu\n");

                user_mode = read_valid_u8("Choice: ");

                if (user_mode == 1) view_patient_record();
                else if (user_mode == 2) view_reservation();
                else if (user_mode == 3) break; // Return to Main Menu
                else printf("Invalid choice! Please try again.\n");
            }
        } 
        else if (mode_choice == 3) {
            printf("\nThank you for using Clinic Management System. Goodbye!\n");
            break;
        } 
        else {
            printf("\nInvalid choice! Please try again.\n");
        }
    }

    free_all_patients();
    return 0;
}
/* Clear leftover characters from the input buffer */
void flush_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Prompt repeatedly until a valid unsigned short is entered */
u16 read_valid_u16(const char *prompt) {
    u16 value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%hu", &value) == 1) {
            flush_input();
            return value;
        }
        printf("Invalid input! Please enter a valid number.\n");
        flush_input();
    }
}

/* Prompt repeatedly until a valid unsigned char is entered */
u8 read_valid_u8(const char *prompt) {
    u8 value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%hhu", &value) == 1) {
            flush_input();
            return value;
        }
        printf("Invalid input! Please enter a valid number.\n");
        flush_input();
    }
}

/* Free all nodes in the patient linked list */
void free_all_patients(void) {
    patient *cur = head;
    while (cur != NULL) {
        patient *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    head = NULL;
}

/* Admin authentication with 3 trials limit */
u8 admin_login(void) {
    u16 password;
    u8 trials = 0;
    while (trials < 3) {
        printf("Enter password: ");
        if (scanf("%hu", &password) == 1) {
            flush_input();
            if (password == 1234) {
                printf("Login successful!\n");
                return 1;
            }
        } else {
            flush_input();
        }
        trials++;
        printf("Incorrect password, remaining trials: %d\n", 3 - trials);
    }
    printf("All trials incorrect. System shutting down...\n");
    return 0;
}

/* Search patient by ID */
patient *find_patient(u16 id) {
    patient *cur = head;
    while (cur != NULL) {
        if (cur->ID == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

/* Add new patient record */
void add_patient(void) {
    u16 scan_id = read_valid_u16("Please enter patient ID: ");

    if (scan_id == 0) {
        printf("Invalid ID! ID must be greater than 0.\n");
        return;
    }

    if (find_patient(scan_id) != NULL) {
        printf("Error: Patient ID already exists.\n");
        return;
    }

    patient *new_patient = (patient *)malloc(sizeof(patient));
    if (new_patient == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    new_patient->ID = scan_id;

    while (1) {
        printf("Please enter patient name: ");
        if (scanf(" %49[^\n]", new_patient->name) == 1 && new_patient->name[0] != '\0') {
            flush_input();
            break;
        }
        printf("Invalid name! Please enter a valid non-empty name.\n");
        flush_input();
    }

    while (1) {
        u16 tmp_age = read_valid_u16("Please enter patient age (1-120): ");
        if (tmp_age >= 1 && tmp_age <= 120) {
            new_patient->age = (u8)tmp_age;
            break;
        }
        printf("Invalid age! Must be between 1 and 120.\n");
    }

    while (1) {
        printf("Please enter patient gender (M/F): ");
        char g;
        if (scanf(" %c", &g) == 1) {
            flush_input();
            g = (char)toupper((unsigned char)g);
            if (g == 'M' || g == 'F') {
                new_patient->gender = (u8)g;
                break;
            }
        } else {
            flush_input();
        }
        printf("Invalid gender! Please enter M or F.\n");
    }

    new_patient->next = head;
    head = new_patient;
    printf("Patient record added successfully!\n");
}

/* Edit existing patient record */
void edit_patient(void) {
    u16 scan_id = read_valid_u16("Please enter patient ID: ");

    patient *target = find_patient(scan_id);
    if (target == NULL) {
        printf("Error: Patient ID does not exist.\n");
        return;
    }

    printf("Current Info -> Name: %s | Age: %hhu | Gender: %c\n", target->name, target->age, target->gender);

    while (1) {
        printf("Please enter new patient name: ");
        if (scanf(" %49[^\n]", target->name) == 1 && target->name[0] != '\0') {
            flush_input();
            break;
        }
        printf("Invalid name! Please enter a valid non-empty name.\n");
        flush_input();
    }

    while (1) {
        u16 tmp_age = read_valid_u16("Please enter new patient age (1-120): ");
        if (tmp_age >= 1 && tmp_age <= 120) {
            target->age = (u8)tmp_age;
            break;
        }
        printf("Invalid age! Must be between 1 and 120.\n");
    }

    while (1) {
        printf("Please enter new patient gender (M/F): ");
        char g;
        if (scanf(" %c", &g) == 1) {
            flush_input();
            g = (char)toupper((unsigned char)g);
            if (g == 'M' || g == 'F') {
                target->gender = (u8)g;
                break;
            }
        } else {
            flush_input();
        }
        printf("Invalid gender! Please enter M or F.\n");
    }

    printf("Patient record updated successfully!\n");
}

/* Delete patient record and clear associated reservation */
void delete_patient(void) {
    u16 scan_id = read_valid_u16("Please enter patient ID to delete: ");

    patient *cur = head;
    patient *prev = NULL;

    while (cur != NULL) {
        if (cur->ID == scan_id) break;
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        printf("Error: Patient ID does not exist.\n");
        return;
    }

    // Cancel reservation if exists
    for (u8 i = 0; i < Total_SLOT; i++) {
        if (slot_reservation[i] == scan_id) {
            slot_reservation[i] = 0;
            break;
        }
    }

    if (prev == NULL) {
        head = cur->next;
    } else {
        prev->next = cur->next;
    }

    free(cur);
    printf("Patient record and associated reservation deleted successfully!\n");
}

/* Reserve a doctor slot */
void reserve_slot(void) {
    u16 scan_id = read_valid_u16("Please enter patient ID: ");

    if (find_patient(scan_id) == NULL) {
        printf("Error: Patient ID does not exist.\n");
        return;
    }

    for (u8 i = 0; i < Total_SLOT; i++) {
        if (slot_reservation[i] == scan_id) {
            printf("Error: This patient already has a reservation. Cancel it first.\n");
            return;
        }
    }

    printf("\n=== Available Slots ===\n");
    u8 available_count = 0;
    const char *slot_times[Total_SLOT] = {
        "1. 2:00pm to 2:30pm",
        "2. 2:30pm to 3:00pm",
        "3. 3:00pm to 3:30pm",
        "4. 4:00pm to 4:30pm",
        "5. 4:30pm to 5:00pm"
    };

    for (u8 i = 0; i < Total_SLOT; i++) {
        if (slot_reservation[i] == 0) {
            printf("%s\n", slot_times[i]);
            available_count++;
        }
    }

    if (available_count == 0) {
        printf("Sorry, no available slots for today!\n");
        return;
    }

    u8 slot_choice = read_valid_u8("Choose a slot number (1-5): ");

    if (slot_choice >= 1 && slot_choice <= Total_SLOT) {
        if (slot_reservation[slot_choice - 1] == 0) {
            slot_reservation[slot_choice - 1] = scan_id;
            printf("Slot reserved successfully!\n");
        } else {
            printf("Error: Selected slot is already reserved.\n");
        }
    } else {
        printf("Error: Invalid slot choice!\n");
    }
}

/* Cancel an existing slot reservation */
void cancel_reservation(void) {
    u16 scan_id = read_valid_u16("Please enter patient ID to cancel reservation: ");

    if (find_patient(scan_id) == NULL) {
        printf("Error: Patient ID does not exist.\n");
        return;
    }

    u8 found_reservation = 0;
    for (u8 i = 0; i < Total_SLOT; i++) {
        if (slot_reservation[i] == scan_id) {
            slot_reservation[i] = 0;
            found_reservation = 1;
            printf("Reservation for slot %d cancelled successfully!\n", i + 1);
            break;
        }
    }

    if (!found_reservation) {
        printf("No active reservation found for this patient ID.\n");
    }
}

/* View specific patient details */
void view_patient_record(void) {
    u16 scan_id = read_valid_u16("Please enter patient ID: ");

    patient *p = find_patient(scan_id);
    if (p == NULL) {
        printf("Error: Patient ID does not exist.\n");
        return;
    }

    printf("\n--- Patient Info ---\n");
    printf("ID     : %hu\n", p->ID);
    printf("Name   : %s\n", p->name);
    printf("Age    : %hhu\n", p->age);
    printf("Gender : %c\n", p->gender);
}

/* View all 5 slots and their current reservation status */
void view_reservation(void) {
    printf("\n=== Today's Reservations Status ===\n");
    const char *slot_times[Total_SLOT] = {
        "1. 2:00pm to 2:30pm",
        "2. 2:30pm to 3:00pm",
        "3. 3:00pm to 3:30pm",
        "4. 4:00pm to 4:30pm",
        "5. 4:30pm to 5:00pm"
    };

    for (u8 i = 0; i < Total_SLOT; i++) {
        if (slot_reservation[i] != 0) {
            printf("%s -> Reserved (Patient ID: %hu)\n", slot_times[i], slot_reservation[i]);
        } else {
            printf("%s -> Available\n", slot_times[i]);
        }
    }
}
