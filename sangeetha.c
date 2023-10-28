import tweepy
import psycopg2
from textblob import TextBlob

# Twitter API credentials
consumer_key = 'your_consumer_key'
consumer_secret = 'your_consumer_secret'
access_token = 'your_access_token'
access_token_secret = 'your_access_token_secret'

# Database credentials
db_host = 'your_db_host'
db_name = 'your_db_name'
db_user = 'your_db_user'
db_password = 'your_db_password'

# Connect to Twitter API
auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
auth.set_access_token(access_token, access_token_secret)
api = tweepy.API(auth, wait_on_rate_limit=True)

# Connect to the database
conn = psycopg2.connect(host=db_host, database=db_name, user=db_user, password=db_password)
cursor = conn.cursor()

# Define the keyword or hashtag you want to search for
search_query = "your_search_query"

# Collect tweets
tweets = tweepy.Cursor(api.search, q=search_query, lang="en").items(100)  # Example: Collect 100 tweets

for tweet in tweets:
    tweet_text = tweet.text
    user_followers = tweet.user.followers_count
    user_friends = tweet.user.friends_count
    
    # Perform sentiment analysis
    analysis = TextBlob(tweet_text)
    sentiment_score = analysis.sentiment.polarity

    # Store data in the database
    cursor.execute("INSERT INTO tweets (text, user_followers, user_friends, sentiment_score) VALUES (%s, %s, %s, %s)",
                   (tweet_text, user_followers, user_friends, sentiment_score))
    conn.commit()

# Calculate and print analytics (this is simplified)
cursor.execute("SELECT AVG(sentiment_score) FROM tweets")
average_sentiment = cursor.fetchone()[0]

cursor.execute("SELECT COUNT(*) FROM tweets")
tweet_count = cursor.fetchone()[0]

print(f"Average sentiment score: {average_sentiment}")
print(f"Total number of tweets collected: {tweet_count}")

# Close the database connection
conn.close()
