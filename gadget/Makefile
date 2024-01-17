CONTAINER_REPO ?= ghcr.io/alban/signal-go-gadget
IMAGE_TAG ?= latest

.PHONY: build
build:
	IG_EXPERIMENTAL=true sudo -E \
		ig image build \
			-t $(CONTAINER_REPO):$(IMAGE_TAG) \
			--update-metadata .

.PHONY: run
run:
	IG_EXPERIMENTAL=true sudo -E \
		ig run $(CONTAINER_REPO):$(IMAGE_TAG) $$PARAMS

.PHONY: push
push:
	IG_EXPERIMENTAL=true sudo -E \
		ig image push $(CONTAINER_REPO):$(IMAGE_TAG)
	

