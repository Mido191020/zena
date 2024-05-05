import requests
from bs4 import BeautifulSoup
import csv
import pandas as pd

website = 'https://www.yallakora.com/match-center/?date=11/23/2022'
page = requests.get(website)

def main(page):
    src = page.content
    soup = BeautifulSoup(src, 'html.parser')
    matches_details = []
    championships = soup.find_all("div", {"class": "matchcard"})
    for championship in championships:
        championship_title = championship.contents[1].find('h2').text.strip()
        all_matches = championship.contents[3].find_all('li')
        number_of_matches = len(all_matches)
        for i in range(number_of_matches):
            # get teams names
            team_a = all_matches[i].find('div', {'class': 'teamA'}).text.strip()
            team_b = all_matches[i].find('div', {'class': 'teamB'}).text.strip()
            # get score
            match_result = all_matches[i].find('div', {'class': "MResult"}).find_all('span', {'class': 'score'})
            score = f'{match_result[0].text.strip()} - {match_result[1].text.strip()}'
            # get match time
            match_time = all_matches[i].find("div", {'class': "MResult"}).find('span', {'class': 'time'}).text.strip()
            # add match info
            matches_details.append({'Championship': championship_title, 'Team A': team_a, 'Team B': team_b, 'Score': score, 'Time': match_time})
    return matches_details

if __name__ == "__main__":
    matches = main(page)
    with open('matches.csv', 'w', newline='', encoding='utf-8') as csvfile:
        fieldnames = ['Championship', 'Team A', 'Team B', 'Score', 'Time']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for match in matches:
            writer.writerow(match)

    # Read the CSV file into a DataFrame
    df = pd.read_csv('matches.csv')

    # Display the DataFrame
    print(df)
