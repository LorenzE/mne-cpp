#!/bin/bash

if [[ "${BUILD_REASON}" != "PullRequest" ]]; then
    # === Deployment ===
    TANGIBLES=(mne_scan mne_browse mne_analyze)
    n_elements=${#TANGIBLES[@]}
	
    # === Copy Tangibles ===
    for ((i = 0; i < n_elements; i++)); do
        cp "./bin/${TANGIBLES[i]}.dmg" "./"
        tangible_name="${TANGIBLES[i]}-$TRAVIS_BRANCH.dmg"
        mv "./${TANGIBLES[i]}.dmg" $tangible_name
        # upload artifacts
        curl -u $MASTER_LOGIN:$MASTER_PASSWORD -T $tangible_name ftp://$REMOTE_SERVER/
    done
fi