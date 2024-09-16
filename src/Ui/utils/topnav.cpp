#include "topnav.hpp"
#include "align.hpp"
#include "box.hpp"
#include "column.hpp"
#include "gestureDetector.hpp"
#include "observer.hpp"
#include "row.hpp"
#include "stack.hpp"
#include "text.hpp"
#include <GLFW/glfw3.h>

using namespace squi;

struct TopNavButton {
	// Args
	squi::Widget::Args widget{};
	std::string_view name;
	int64_t pageId;
	Observable<int64_t> pageChanger;
	VoidObservable readyObserver;

	struct Storage {
		// Data
	};

	operator squi::Child() const {
		auto storage = std::make_shared<Storage>();

		return GestureDetector{
			.onClick = [pageChanger = pageChanger, pageId = pageId](auto) {
				pageChanger.notify(pageId);
			},
			.child = Stack{
				.widget{
					.width = Size::Shrink,
					.height = 40.f,
					.sizeConstraints{
						.minWidth = 48.f,
					},
					.padding = Padding{12.f, 0.f},
				},
				.children{
					Align{
						.child = Text{
							.text = name,
						},
					},
					Align{
						.yAlign = 1.f,
						.child = Box{
							.widget{
								.width = 16.f,
								.height = 3.f,
								.onInit = [readyObserver = readyObserver, pageChanger = pageChanger, pageId = pageId](Widget &w) {
									w.customState.add(pageChanger.observe([&w, pageId = pageId](int64_t page) {
										if (page == pageId) {
											w.flags.visible = true;
										} else {
											w.flags.visible = false;
										}
									}));

									readyObserver.notify();
								},
							},
							.color{0x60CDFFFF},
							.borderRadius{2.f},
						},
					},

				},
			},
		};
	}
};

UI::TopNav::operator squi::Child() const {
	auto storage = std::make_shared<Storage>(Storage{
		.quota = pages.size() * 2,
		.ready = 0,
		.maxPage = static_cast<int64_t>(pages.size()) - 1,
	});

	VoidObservable readyObserver{};

	storage->readyObserver = readyObserver.observe([startIndex = startIndex, pageChanger = pageChanger, weakStorage = std::weak_ptr<Storage>(storage)]() {
		if (auto storage = weakStorage.lock()) {
			storage->ready++;
			if (storage->ready >= storage->quota) pageChanger.notify(startIndex);
		}
	});

	storage->pageObserver = pageChanger.observe([weakStorage = std::weak_ptr(storage)](int64_t page) {
		if (auto storage = weakStorage.lock()) {
			storage->currentPage = page;
		}
	});

	return Column{
		.widget = widget,
		.children{
			Row{
				.widget{
					.height = 48.f,
					.padding = Padding{0.f, 4.f},
					.customState{storage},
					.onInit = [readyObserver = readyObserver, pages = pages, pageChanger = pageChanger](Widget &w) {
						for (const auto &[index, page]: std::views::enumerate(pages)) {
							w.addChild(TopNavButton{
								.name = page.name,
								.pageId = index,
								.pageChanger = pageChanger,
								.readyObserver = readyObserver,
							});
						}
					},
					.onUpdate = [storage](Widget &w) {
						auto &inputState = InputState::of(&w);
						if (inputState.isKeyPressedOrRepeat(GLFW_KEY_TAB, GLFW_MOD_CONTROL)) {
							if (storage->currentPage < storage->maxPage) {
								storage->pageObserver.notifyOthers(storage->currentPage + 1);
							}
						}
						if (inputState.isKeyPressedOrRepeat(GLFW_KEY_TAB, GLFW_MOD_CONTROL | GLFW_MOD_SHIFT)) {
							if (storage->currentPage > 0) {
								storage->pageObserver.notifyOthers(storage->currentPage - 1);
							}
						}
					},
				},
			},
			Box{
				.widget{
					.height = 1.f,
				},
				.color{0.f, 0.f, 0.f, 0.1f},
			},
			Box{
				.widget{
					.onInit = [readyObserver = readyObserver, pageChanger = pageChanger, pages = pages](Widget &w) {
						for (const auto &[index, page]: std::views::enumerate(pages)) {
							page.content->customState.add(pageChanger.observe([&w = *page.content, pageId = index](int64_t page) {
								if (page == pageId) {
									w.flags.visible = true;
								} else {
									w.flags.visible = false;
								}
							}));
							readyObserver.notify();
							w.addChild(page.content);
						}
					},
				},
				.color{0x3A3A3A4C},
			},
		},
	};
}
