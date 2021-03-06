set -e -u

cat countries_to_generate.txt | while read country; do

    DATA_PATH="Countries/$country/content/data"
    # copy js, css
    cp -r ../../data/* "$DATA_PATH"

    echo "Generate .zip for : $country"

    # zip content with pushd to avoid adding full path
    pushd "Countries/$country/content"
      STRIPPED="${country/_(*/}"
      rm "../$STRIPPED.data.zip" || true
      zip -r "../${STRIPPED}.data.zip" data
    popd

    echo "Zipped $country"
done
