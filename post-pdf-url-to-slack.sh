curl -X POST \
  -d @post-pdf-url-to-slack-payload.json \
  https://hooks.slack.com/services/$1
