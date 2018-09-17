#!/bin/sh

if [ $# != 2 ]
then
	echo 'Usage: upload_dropbox.sh <file> <token>'
	exit 1
fi

filename="book.pdf"

echo "Upload $1 to $filename"

curl --fail -X POST https://content.dropboxapi.com/2/files/upload \
	--header "Authorization: Bearer $2" \
	--header "Dropbox-API-Arg: {\"path\": \"/$filename\",\"mode\": \"overwrite\",\"autorename\": true,\"mute\": false}" \
	--header "Content-Type: application/octet-stream" \
	--data-binary @$1 \
	|| exit 1

echo 
echo "Success!"
