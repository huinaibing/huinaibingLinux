time=$(date "+%Y%m%d-%H%M%S")

echo "${time}"


git add .
git commit -m "push by sh: ${time}"
git push 
