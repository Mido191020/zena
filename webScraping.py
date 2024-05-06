import requests
from bs4 import BeautifulSoup
import csv

def scraper(championships):
    championship_title = championships.contents[1].find('h2').text.strip()
    all_matches = championships.contents[3].find_all("div", {"class": "liItem"})
    num_matches = len(all_matches)

    for match in range(num_matches):
        teamA = all_matches[match].find("div", {"class": "teamA"}).text.strip()
        teamB = all_matches[match].find("div", {"class": "teamB"}).text.strip()
        match_result = all_matches[match].find("div", {"class": "MResult"}).find_all("span", {"class": "score"})
        score = f"{match_result[0].text.strip()} - {match_result[1].text.strip()}"
        time = all_matches[match].find("div", {"class": "MResult"}).find("span", {"class": "time"}).text.strip()
        matches_details.append({"Championship": championship_title, "Team A": teamA, "Team B": teamB, "Score": score, "Time": time})

matches_details = []

def main(date):
    website = f"https://yallakora.com/match-center/?date={date}"
    page = requests.get(website)
    soup = BeautifulSoup(page.content, 'lxml')
    championships = soup.find_all("div", {"class": "matchesList"})

    if len(championships) >= 1:
        for i in range(len(championships)):
            scraper(championships[i])
    else:
        print("There is no available championships")

def date_range(start_year, end_year):
    for year in range(start_year, end_year + 1):
        for month in range(1, 13):
            for day in range(1, 32):
                try:
                    main(f'{day}/{month}/{year}')
                    if len(matches_details) >= 1000:
                        return
                except:
                    pass

date_range(2023, 2025)

def save_to_csv(data, filename):
    with open(filename, 'w', newline='', encoding='utf-8') as csvfile:
        fieldnames = ['Championship', 'Team A', 'Team B', 'Score', 'Time']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for item in data:
            writer.writerow(item)

save_to_csv(matches_details, 'matches2.csv')

import pandas as pd
import arabic_reshaper
from bidi.algorithm import get_display
import matplotlib.pyplot as plt

df = pd.read_csv('matches.csv')
df.dropna(inplace=True)
df['Team A'] = df['Team A'].str.strip()
df['Team B'] = df['Team B'].str.strip()
df['Team A'] = df['Team A'].apply(lambda x: get_display(arabic_reshaper.reshape(x)))
df['Team B'] = df['Team B'].apply(lambda x: get_display(arabic_reshaper.reshape(x)))
df[['Score_A', 'Score_B']] = df['Score'].str.split(' - ', expand=True)
df['Score_A'] = pd.to_numeric(df['Score_A'], errors='coerce')
df['Score_B'] = pd.to_numeric(df['Score_B'], errors='coerce')

team_scores = df.groupby('Team A')['Score_A'].sum().add(df.groupby('Team B')['Score_B'].sum(), fill_value=0)
top_ten_teams = team_scores.sort_values(ascending=False).head(10)

plt.figure(figsize=(10, 6))
top_ten_teams.plot(kind='bar')
plt.title('Top Ten Teams with the Highest Total Scores')
plt.xlabel('Team')
plt.ylabel('Total Score')
plt.xticks(rotation=45, ha='right')
plt.tight_layout()
plt.show()

matches_count = pd.concat([df['Team A'], df['Team B']]).value_counts()
top_ten_teams = matches_count.head(10)

plt.figure(figsize=(10, 6))
top_ten_teams.plot(kind='bar')
plt.title('Top 10 Teams with the Most Matches Played')
plt.xlabel('Team')
plt.ylabel('Number of Matches')
plt.xticks(rotation=45, ha='right')
plt.tight_layout()
plt.show()
