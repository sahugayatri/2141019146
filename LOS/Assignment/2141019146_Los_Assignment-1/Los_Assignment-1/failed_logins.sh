#!/bin/bash

# File to store extracted login attempts
output_file="login_attempts_report.txt"

# Parse auth log for login attempts, extract user and error messages
grep "Failed password" /var/log/auth.log | awk '{print $1, $2, $3, $9, $11}' | sed 's/invalid user //g' > "$output_file"

echo "Login attempts extracted to $output_file"
