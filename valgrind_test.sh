mkdir download_test
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./steamloader 734155390 731218653 731614240 732123864 730734187 731468897 731258574 732762723 731954308 -o download_test/
